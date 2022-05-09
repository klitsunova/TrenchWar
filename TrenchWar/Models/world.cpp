#include <utility>

#include "world.h"

World::World(const QString& path) {
  LoadMap(path);
  cells_.resize(size_.width(),
                std::vector<Cell>(size_.height()));
  picture_ = DrawWorld();
}

const std::vector<std::shared_ptr<Soldier>>& World::GetSoldiers() const {
  return soldiers_;
}

std::vector<std::shared_ptr<Soldier>>& World::GetSoldiers() {
  return soldiers_;
}

void World::AddSoldier() {
  auto new_object = std::make_shared<Soldier>();
  new_object->SetRandomPosition(size_);
  soldiers_.push_back(new_object);
  game_objects_.push_back(new_object);
}

void World::AddSoldier(const QPoint& position, bool type) {
  assert(position.x() >= 0 && position.x() < size_.width());
  assert(position.y() >= 0 && position.x() < size_.height());
  auto new_object = std::make_shared<Soldier>(position, type);
  soldiers_.push_back(new_object);
  game_objects_.push_back(new_object);
}

void World::AddTerrainObject() {
  auto new_object = std::make_shared<TerrainObject>();
  new_object->SetRandomPosition(size_);
  QPoint pos = new_object->GetPosition();
  cells_[pos.x()][pos.y()].terrain_objects.push_back(new_object);
  game_objects_.push_back(new_object);
}

const std::vector<std::shared_ptr<GameObject>>& World::GetGameObjects() const {
  return game_objects_;
}

std::vector<std::shared_ptr<GameObject>>& World::GetGameObjects() {
  return game_objects_;
}

const QSize& World::GetSize() const {
  return size_;
}

const World::Cell& World::GetCell(const QPoint& point) const {
  assert(point.x() >= 0 && point.x() < cells_.size());
  assert(point.y() >= 0 && point.y() < cells_[point.x()].size());
  return cells_[point.x()][point.y()];
}

World::Cell& World::GetCell(const QPoint& point) {
  assert(point.x() >= 0 && point.x() < cells_.size());
  assert(point.y() >= 0 && point.y() < cells_[point.x()].size());
  return cells_[point.x()][point.y()];
}

const QPixmap& World::GetPixmap() const {
  return picture_;
}

void World::UpdateDistances() {
  if (!is_need_update_distances) {
    return;
  }
  is_need_update_distances = false;

  for (int i = 0; i < size_.width(); ++i) {
    for (int j = 0; j < size_.height(); ++j) {
      cells_[i][j].used = false;
      cells_[i][j].distance = INT32_MAX;
    }
  }

  auto cmp =
      [&](std::pair<int, int> left, std::pair<int, int> right) {
        return cells_[left.first][left.second].distance
            > cells_[right.first][right.second].distance;
      };
  std::priority_queue<std::pair<int, int>,
                      std::vector<std::pair<int, int>>,
                      decltype(cmp)>
      latest(cmp);

  for (auto& soldier : soldiers_) {
    int x = soldier->GetPosition().x();
    int y = soldier->GetPosition().y();
    if (!soldier->IsDefender()) {
      continue;
    }
    cells_[x][y].distance = 0;
    latest.push(std::make_pair(x, y));
  }

  auto push_if =
      [&](int x, int y, int dist, bool condition = true) {
        if (!condition) {
          return;
        }
        if (!cells_[x][y].used && cells_[x][y].distance > dist) {
          cells_[x][y].distance = dist;
          latest.push(std::make_pair(x, y));
        }
      };

  while (!latest.empty()) {
    int x = latest.top().first;
    int y = latest.top().second;
    int current_distance = cells_[x][y].distance
        + cells_[x][y].landscape.speed_characteristic;

    // left neighbor
    push_if(x - 1, y, current_distance, (x != 0));
    // right neighbor
    push_if(x + 1, y, current_distance, (x != size_.width() - 1));
    // upper neighbor
    push_if(x, y - 1, current_distance, (y != 0));
    // lower neighbor
    push_if(x, y + 1, current_distance, (y != size_.height() - 1));

    cells_[x][y].used = true;
    latest.pop();
  }
}

void World::MoveSoldiers() {
  UpdateDistances();

  auto move_if = [&](int soldier_pos, int& current_dist,
                     int to_x, int to_y, int condition = true) {
    if (!condition) {
      return;
    }
    if (current_dist > cells_[to_x][to_y].distance) {
      current_dist = cells_[to_x][to_y].distance;
      soldiers_[soldier_pos]->SetPosition(QPoint(to_x, to_y));
    }
  };

  for (int i = 0; i < soldiers_.size(); ++i) {
    if (soldiers_[i]->IsDefender()) {
      continue;
    }
    int x = soldiers_[i]->GetPosition().x();
    int y = soldiers_[i]->GetPosition().y();
    int distance = cells_[x][y].distance;
    // left neighbor
    move_if(i, distance, x - 1, y, x != 0);
    // right neighbor
    move_if(i, distance, x + 1, y, x != size_.width() - 1);
    // upper neighbor
    move_if(i, distance, x, y - 1, y != 0);
    // lower neighbor
    move_if(i, distance, x, y + 1, y != size_.height() - 1);
  }
}

void World::LoadMap(const QString& path) {
  QFile file(path);

  if (!file.open(QIODevice::ReadOnly)) {
    qCritical("Cannot open file for reading");
  }
  QTextStream in(&file);
  std::vector<std::pair<int64_t, int>> color_and_value;

  QString size = in.readLine();
  int size_t = size.toInt();
  for (int i = 0; i < size_t; i++) {
    int64_t color;
    int value;
    in >> color >> value;
    color_and_value.emplace_back(color, value);
  }
  in.readLine();

  int length, width;
  in >> length >> width;
  size_ = QSize(width, length);
  cells_.resize(size_.width(),
                std::vector<Cell>(size_.height()));

  for (int i = 0; i < length; ++i) {
    for (int j = 0; j < width; ++j) {
      int color_index;
      in >> color_index;
      cells_[i][j].landscape = Landscape(color_and_value[color_index].first,
                                         color_and_value[color_index].second);
    }
  }

  file.close();
}

QPixmap World::DrawWorld() const {
  QPixmap picture(size_);
  auto painter = QPainter(&picture);
  int window_width = painter.window().width() - 1;
  int window_height = painter.window().height() - 1;
  for (int i = 0; i < size_.width(); ++i) {
    for (int j = 0; j < size_.height(); ++j) {
      int x_top = (window_width * i) / size_.width();
      int x_bottom = ((window_width * (i + 1)) / size_.width());
      int y_top = (window_height * j) / size_.height();
      int y_bottom = ((window_height * (j + 1)) / size_.height());
      QRect cell_rect(QPoint(x_top, y_top),
                      QPoint(x_bottom, y_bottom));
      QColor color = cells_[j][i].landscape.color;
      painter.setBrush(QBrush(color));
      painter.setPen(QPen(QColor(color), 1));
      painter.drawRect(cell_rect);
    }
  }
  return picture;
}

World::Landscape::Landscape(const QColor& q_color, int speed) {
  color = q_color;
  speed_characteristic = speed;
}

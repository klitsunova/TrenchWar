#include <utility>

#include "world.h"

World::World(QSize size)
    : size_(size) {
  cells_.resize(size.width(),
                std::vector<Cell>(size.height()));
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
  for (int i = 0; i < size_.width(); ++i) {
    for (int j = 0; j < size_.height(); ++j) {
      cells_[i][j].used = false;
      cells_[i][j].distance = INT32_MAX;
    }
  }

  std::queue<std::pair<int, int>> latest;
  for (int i = 0; i < soldiers_.size(); ++i) {
    int x = soldiers_[i]->GetPosition().x();
    int y = soldiers_[i]->GetPosition().y();
    cells_[x][y].used = true;
    if (!soldiers_[i]->GetType()) {
      continue;
    }
    cells_[x][y].distance = 0;
    latest.push(std::make_pair(x, y));
  }

  while (!latest.empty()) {
    int x = latest.front().first;
    int y = latest.front().second;
    // to do "+1"
    // left neighbor
    if (x != 0) {
      if (!cells_[x - 1][y].used && cells_[x - 1][y].distance > cells_[x][y].distance + 1) {
        cells_[x - 1][y].distance = cells_[x][y].distance + 1;
        latest.push(std::make_pair(x - 1, y));
      }
    }
    // right neighbor
    if (x != size_.width() - 1) {
      if (!cells_[x + 1][y].used && cells_[x + 1][y].distance > cells_[x][y].distance + 1) {
        cells_[x + 1][y].distance = cells_[x][y].distance + 1;
        latest.push(std::make_pair(x + 1, y));
      }
    }
    // upper neighbor
    if (y != 0) {
      if (!cells_[x][y - 1].used && cells_[x][y - 1].distance > cells_[x][y].distance + 1) {
        cells_[x][y - 1].distance = cells_[x][y].distance + 1;
        latest.push(std::make_pair(x, y - 1));
      }
    }
    // lower neighbor
    if (y != size_.height() - 1) {
      if (!cells_[x][y + 1].used && cells_[x][y + 1].distance > cells_[x][y].distance + 1) {
        cells_[x][y + 1].distance = cells_[x][y].distance + 1;
        latest.push(std::make_pair(x, y + 1));
      }
    }
    latest.pop();
  }
}

void World::MoveSoldiers() {
  for (int i = 0; i < soldiers_.size(); ++i) {
    if (soldiers_[i]->GetType()) {
      continue;
    }
    UpdateDistances();
    int x = soldiers_[i]->GetPosition().x();
    int y = soldiers_[i]->GetPosition().y();
    // to do
    // if (cells_[x][y].distance == 1) {
    //   continue;
    // }
    int distance = INT32_MAX;
    // left neighbor
    if (x != 0 && !cells_[x - 1][y].used) {
      distance = std::min(distance, cells_[x - 1][y].distance);
    }
    // right neighbor
    if (x != (size_.width() - 1) && !cells_[x + 1][y].used) {
      distance = std::min(distance, cells_[x + 1][y].distance);
    }
    // upper neighbor
    if (y != 0 && !cells_[x][y - 1].used) {
      distance = std::min(distance, cells_[x][y - 1].distance);
    }
    // lower neighbor
    if (y != (size_.height() - 1) && !cells_[x][y + 1].used) {
      distance = std::min(distance, cells_[x][y + 1].distance);
    }

    // left neighbor
    if (x != 0 && !cells_[x - 1][y].used) {
      if (cells_[x - 1][y].distance == distance) {
        soldiers_[i]->MoveLeft();
        continue;
      }
    }
    // right neighbor
    if (x != size_.width() - 1 && !cells_[x + 1][y].used) {
      if (cells_[x + 1][y].distance == distance) {
        soldiers_[i]->MoveRight();
        continue;
      }
    }
    // upper neighbor
    if (y != 0 && !cells_[x][y - 1].used) {
      if (cells_[x][y - 1].distance == distance) {
        soldiers_[i]->MoveUp();
        continue;
      }
    }
    // lower neighbor
    if (y != size_.height() - 1 && !cells_[x][y + 1].used) {
      if (cells_[x][y + 1].distance == distance) {
        soldiers_[i]->MoveDown();
        continue;
      }
    }
  }
}

QPixmap World::DrawWorld() const {
  QPixmap picture(size_);
  auto painter = QPainter(&picture);
  int window_width = painter.window().width() - 1;
  int window_height = painter.window().height() - 1;

  painter.setBrush(QBrush("#ff28e024")); // temporary
  painter.setPen(QPen(QColor("#ff28e024"), 1));
  for (int i = 0; i < size_.width(); ++i) {
    for (int j = 0; j < size_.height(); ++j) {
      int x_top = (window_width * i) / size_.width();
      int x_bottom = ((window_width * (i + 1)) / size_.width());
      int y_top = (window_height * j) / size_.height();
      int y_bottom = ((window_height * (j + 1)) / size_.height());
      QRect cell_rect(QPoint(x_top, y_top),
                      QPoint(x_bottom, y_bottom));
      QColor color = landscapes_[j][i].color;
      painter.setBrush(QBrush(color));
      painter.setPen(QPen(QColor(color), 1));
      painter.drawRect(cell_rect);
    }
  }
  return picture;
}

void World::LoadMap(const QString& path) {
  QFile file(path);

  if (!file.open(QIODevice::ReadOnly)) {
    qCritical("Cannot open file for reading");
  }
  QTextStream in(&file);
  std::vector<std::pair<int64_t, int>> color_and_value;

  QString size = in.readLine();
  int size_t = std::stoi(size.toStdString());
  for (int i = 0; i < size_t; i++) {
    std::string s = in.readLine().toStdString();
    int index = s.find_first_of(' ');
    std::string s1 = s.substr(0, index);
    std::string s2 = s.substr(index + 1, s.length() - index - 1);
    color_and_value.emplace_back(std::stoll(s1), std::stoi(s2));
  }
  in.readLine();

  std::string sizes = in.readLine().toStdString();
  int index_1 = sizes.find_first_of(' ');
  std::string s1 = sizes.substr(0, index_1);
  std::string s2 = sizes.substr(index_1 + 1, sizes.length() - index_1 - 1);
  int length = std::stoi(s1);
  int width = std::stoi(s2);
  landscapes_.resize(length);
  size_.setHeight(length);
  size_.setWidth(width);

  for (int i = 0; i < length; ++i) {
    std::string s = in.readLine().toStdString();
    int start_index = 0;
    int end_index = 0;
    int index = 0;
    while (start_index < s.length()) {
      end_index = s.find(' ', end_index);
      if (end_index == std::string::npos) {
        end_index = s.length();
      }
      int color_index = std::stoi(s.substr(start_index,
                                           end_index - start_index));
      landscapes_[i].push_back(Landscape(color_and_value[color_index].first,
                                         color_and_value[color_index].second));
      index++;
      end_index++;
      start_index = end_index;
    }
  }

  file.close();
}

World::Landscape::Landscape(const QColor& q_color, int speed) {
  color = q_color;
  speed_characteristic = speed;
}

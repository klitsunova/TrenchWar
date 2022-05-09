#include <utility>

#include "world.h"

World::World(const QString& path) {
  LoadMap(path);
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
    QString s = in.readLine();
    int index = static_cast<int>(s.indexOf(' '));
    long long color = s.mid(0, index).toLongLong();
    int value  = s.mid(index + 1, s.length() - index - 1).toInt();
    color_and_value.emplace_back(color, value);
  }
  in.readLine();

  QString sizes = in.readLine();
  int index_1 = static_cast<int>(sizes.indexOf(' '));
  int length = sizes.mid(0, index_1).toInt();
  int width = sizes.mid(index_1 + 1, sizes.length() - index_1 - 1).toInt();
  size_.setHeight(length);
  size_.setWidth(width);
  cells_.resize(size_.width(),
                std::vector<Cell>(size_.height()));

  for (int i = 0; i < length; ++i) {
    QString s = in.readLine();
    int start_index = 0;
    int end_index = 0;
    int j = 0;
    while (start_index < s.length()) {
      end_index = static_cast<int>(s.indexOf(' ', end_index));
      if (end_index == -1) {
        end_index = static_cast<int>(s.length());
      }
      int color_index = (s.mid(start_index,
                                           end_index - start_index)).toInt();
      cells_[i][j].landscape = Landscape(color_and_value[color_index].first,
                                      color_and_value[color_index].second);
      end_index++;
      start_index = end_index;
      j++;
    }
  }

  file.close();
}

World::Landscape::Landscape(const QColor& q_color, int speed) {
  color = q_color;
  speed_characteristic = speed;
}

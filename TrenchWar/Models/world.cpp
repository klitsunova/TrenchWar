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
    int64_t color;
    int value;
    in >> color >> value;
    color_and_value.emplace_back(color, value);
  }
  in.readLine();

  int length;
  int width;
  in >> length >> width;
  size_.setHeight(length);
  size_.setWidth(width);
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

World::Landscape::Landscape(const QColor& q_color, int speed) {
  color = q_color;
  speed_characteristic = speed;
}

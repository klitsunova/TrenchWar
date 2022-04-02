#include "world.h"

World::World(QSize size) : size_(size) {
  cells_.resize(size.width());
  for (int i = 0; i < size.width(); ++i) {
    cells_[i].resize(size.height());
  }
  picture_ = std::move(DrawWorld());
}

const std::vector<std::shared_ptr<Soldier>>& World::GetSoldiers() const {
  return soldiers_;
}

std::vector<std::shared_ptr<Soldier>>& World::GetSoldiers() {
  return soldiers_;
}

void World::AddSoldier() {
  std::shared_ptr<Soldier> new_object = std::make_shared<Soldier>();
  new_object->SetRandomPosition(size_);
  soldiers_.push_back(new_object);
  game_objects_.push_back(new_object);
}

void World::AddTerrainObject() {
  std::shared_ptr<TerrainObject> new_object =
      std::make_shared<TerrainObject>();
  new_object->SetRandomPosition(size_);
  int x = new_object->GetXPosition();
  int y = new_object->GetYPosition();
  cells_[x][y].terrain_objects.push_back(new_object);
  game_objects_.push_back(new_object);
}

const std::vector<std::shared_ptr<GameObject>>& World::GetGameObjects() const {
  return game_objects_;
}

std::vector<std::shared_ptr<GameObject>>& World::GetGameObjects() {
  return game_objects_;
}

int World::GetWidth() const {
  return size_.width();
}

int World::GetHeight() const {
  return size_.height();
}

QSize World::GetSize() const {
  return size_;
}

const World::cell_& World::GetCell(const QPoint& point) const {
  assert(point.x() >= 0 && point.x() < cells_.size());
  assert(point.y() >= 0 && point.y() < cells_[point.x()].size());
  return cells_[point.x()][point.y()];
}

World::cell_& World::GetCell(const QPoint& point) {
  assert(point.x() >= 0 && point.x() < cells_.size());
  assert(point.y() >= 0 && point.y() < cells_[point.x()].size());
  return cells_[point.x()][point.y()];
}

QPixmap World::GetPixmap() const {
  return picture_;
}

QPixmap World::DrawWorld() const {
  QPixmap picture(size_);
  auto painter = QPainter(&picture);
  int window_width = painter.window().width() - 1;
  int window_height = painter.window().height() - 1;
  painter.setBrush(QBrush("#ff28e024"));
  painter.setPen(QPen(QColor("#ff28e024"), 1));
  for (int i = 0; i < size_.width(); ++i) {
    for (int j = 0; j < size_.height(); ++j) {
      int x_top = (window_width * i) / size_.width();
      int x_bottom = ((window_width * (i + 1)) / size_.width());
      int y_top = (window_height * j) / size_.height();
      int y_bottom = ((window_height * (j + 1)) / size_.height());
      QRect rect_of_each_cell(QPoint(x_top, y_top),
                              QPoint(x_bottom, y_bottom));
      painter.drawRect(rect_of_each_cell);
    }
  }
  return picture;
}

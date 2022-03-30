#include "world.h"

const std::vector<std::shared_ptr<Soldier>>& World::GetSoldiers() const {
  return soldiers_;
}

std::vector<std::shared_ptr<Soldier>>& World::GetSoldiers() {
  return soldiers_;
}

void World::ClearSoldiersFromCells() {
  if (cells_.empty()) {
    return;
  }
  for (int i = 0; i < width_; ++i) {
    for (int j = 0; j < height_; ++j) {
      assert(i < cells_.size());
      assert(j < cells_[i].size());
      while (!cells_[i][j].soldiers.empty()) {
        cells_[i][j].soldiers.pop_back();
      }
    }
  }
}

void World::UpdateSoldiersInCells() {
  if (cells_.empty()) {
    return;
  }
  ClearSoldiersFromCells();
  for (int i = 0; i < soldiers_.size(); ++i) {
    assert(soldiers_[i]->GetXPosition() >= 0
           && soldiers_[i]->GetXPosition() < cells_.size());
    assert(soldiers_[i]->GetYPosition() >= 0 && soldiers_[i]->GetYPosition() < cells_[soldiers_[i]->GetXPosition()].size());
    cells_[soldiers_[i]->GetXPosition()][soldiers_[i]->GetYPosition()].soldiers.push_back(
        soldiers_[i]);
  }
}

void World::AddSoldier() {
  std::shared_ptr<Soldier> new_object = std::make_shared<Soldier>();
  new_object->SetRandomPosition(width_, height_);
  soldiers_.push_back(new_object);
  cells_[new_object->GetXPosition()][new_object->GetYPosition()].soldiers.push_back(
      new_object);
}

void World::AddTerraintbject() {
  std::shared_ptr<TerrainObject> new_object = std::make_shared<TerrainObject>();
  new_object->SetRandomPosition(width_, height_);
  cells_[new_object->GetXPosition()][new_object->GetYPosition()].terrain_objects.push_back(
      new_object);
}

World::World(int width, int height) : width_(width), height_(height) {
  cells_.resize(width);
  for (int i = 0; i < width; ++i) {
    cells_[i].resize(height);
  }
}

int World::GetWidth() const {
  return width_;
}

int World::GetHeight() const {
  return height_;
}

const World::cell_& World::GetCell(int x, int y) const {
  assert(x >= 0 && x < cells_.size());
  assert(y >= 0 && y < cells_[x].size());
  return cells_[x][y];
}

World::cell_& World::GetCell(int x, int y) {
  assert(x >= 0 && x < cells_.size());
  assert(y >= 0 && y < cells_[x].size());
  return cells_[x][y];
}

void World::DrawMap(QPainter* painter) {
  painter->save();
  int window_width = painter->window().width() - 1;
  int window_height = painter->window().height() - 1;
  painter->setBrush(QBrush("#ff28e024"));
  painter->setPen(QPen(QColor("#ff28e024"), 1));
  for (int i = 0; i < width_; ++i) {
    for (int j = 0; j < height_; ++j) {
      int x_pos = (window_width * i) / width_;
      int dx = ((window_width * (i + 1)) / width_) - x_pos;
      int y_pos = (window_height * j) / height_;
      int dy = ((window_height * (j + 1)) / height_) - y_pos;
      QRect rect(x_pos, y_pos, dx, dy);
      QPoint point((window_width * (2 * i + 1)) / (2 * width_),
                   (window_height * (2 * j + 1)) / (2 * height_));
      painter->drawRect(rect);
      cells_[i][j].bounding_rect = rect;
      cells_[i][j].point_on_screen = point;
    }
  }
  //draw soldiers on map
  for (int i = 0; i < width_; ++i) {
    for (int j = 0; j < height_; ++j) {
      for (auto& soldier: cells_[i][j].soldiers) {
        soldier->DrawObject(painter, cells_[i][j].point_on_screen);
      }
      for (auto& terrain_object: cells_[i][j].terrain_objects) {
        terrain_object->DrawObject(painter, cells_[i][j].point_on_screen);
      }
    }
  }
  painter->restore();
}

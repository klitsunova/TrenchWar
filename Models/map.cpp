#include "map.h"

std::vector<std::shared_ptr<GameObject>> Map::GetObjects() const {
  std::vector<std::shared_ptr<GameObject>> objects_;
  for (const auto& soldier : soldiers_) {
    objects_.push_back(soldier);
  }
  for (const auto& terrain_object : terrain_objects_) {
    objects_.push_back(terrain_object);
  }
  return objects_;
}

std::vector<std::shared_ptr<Soldier>> Map::GetSoldiers() const {
  return soldiers_;
}

std::vector<std::shared_ptr<TerrainObject>> Map::GetTerrainObjects() const {
  return terrain_objects_;
}

void Map::AddSoldier() {
  soldiers_.push_back(std::make_shared<Soldier>());
}

void Map::AddTerraintbject() {
  terrain_objects_.push_back(std::make_shared<TerrainObject>());
}

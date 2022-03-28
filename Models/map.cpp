#include "map.h"

std::vector<std::shared_ptr<GameObject>> Map::GetObjects() {
  std::vector<std::shared_ptr<GameObject>> objects;
  for (const auto& soldier : soldiers_) {
    objects.push_back(soldier);
  }
  for (const auto& terrain_object : terrain_objects_) {
    objects.push_back(terrain_object);
  }
  return objects;
}

std::vector<std::shared_ptr<Soldier>> Map::GetSoldiers() {
  return soldiers_;
}

std::vector<std::shared_ptr<TerrainObject>> Map::GetTerrainObjects() {
  return terrain_objects_;
}

void Map::AddSoldier() {
  soldiers_.push_back(std::make_shared<Soldier>());
}

void Map::AddTerraintbject() {
  terrain_objects_.push_back(std::make_shared<TerrainObject>());
}

#include "world.h"

std::vector<std::shared_ptr<GameObject>> World::GetObjects() {
  std::vector<std::shared_ptr<GameObject>> objects;
  for (const auto& soldier : soldiers_) {
    objects.push_back(soldier);
  }
  for (const auto& terrain_object : terrain_objects_) {
    objects.push_back(terrain_object);
  }
  return objects;
}

std::vector<std::shared_ptr<Soldier>> World::GetSoldiers() {
  return soldiers_;
}

std::vector<std::shared_ptr<TerrainObject>> World::GetTerrainObjects() {
  return terrain_objects_;
}

void World::AddSoldier() {
  soldiers_.push_back(std::make_shared<Soldier>());
}

void World::AddTerraintbject() {
  terrain_objects_.push_back(std::make_shared<TerrainObject>());
}

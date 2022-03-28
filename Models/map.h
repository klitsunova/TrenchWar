#pragma once

#include <memory>

#include "GameObjects/soldier.h"
#include "GameObjects/terrain_object.h"

class Map {
 public:
  Map() = default;

  ~Map() = default;

  std::vector<std::shared_ptr<GameObject>> GetObjects() const;
  std::vector<std::shared_ptr<Soldier>> GetSoldiers() const;
  std::vector<std::shared_ptr<TerrainObject>> GetTerrainObjects() const;

  void AddSoldier();
  void AddTerraintbject();

 private:
  std::vector<std::shared_ptr<Soldier>> soldiers_;
  std::vector<std::shared_ptr<TerrainObject>> terrain_objects_;
};

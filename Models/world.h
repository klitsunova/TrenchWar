#pragma once

#include <QPainter>

#include <memory>
#include <vector>

#include "GameObjects/soldier.h"
#include "GameObjects/terrain_object.h"

class World {
 private:
  struct cell_;
 public:
  World() = delete;
  World(int, int);

  ~World() = default;

  // std::vector<std::shared_ptr<GameObject>> GetObjects();
  const std::vector<std::shared_ptr<Soldier>>& GetSoldiers() const;
  std::vector<std::shared_ptr<Soldier>>& GetSoldiers();
  void ClearSoldiersFromCells();
  void UpdateSoldiersInCells();
  // std::vector<std::shared_ptr<TerrainObject>> GetTerrainObjects();
  int GetWidth() const;
  int GetHeight() const;
  const cell_& GetCell(int, int) const;
  cell_& GetCell(int, int);
  void DrawMap(QPainter*);

  void AddSoldier();
  // void AddTerraintbject();

 private:
  int width_, height_;
  struct cell_ {
    std::vector<std::shared_ptr<Soldier>> soldiers;
    std::vector<std::shared_ptr<TerrainObject>> terrain_objects;
    QRect bounding_rect;
  };

  std::vector<std::vector<cell_>> cells_;
  std::vector<std::shared_ptr<Soldier>> soldiers_;
};

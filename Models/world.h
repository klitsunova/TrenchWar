#pragma once

#include <QPainter>

#include <memory>
#include <stack>
#include <vector>

#include "GameObjects/soldier.h"
#include "GameObjects/terrain_object.h"

class World {
 private:
  struct Cell_;

 public:
  explicit World(QSize);

  ~World() = default;

  const std::vector<std::shared_ptr<Soldier>>& GetSoldiers() const;
  std::vector<std::shared_ptr<Soldier>>& GetSoldiers();

  const std::vector<std::shared_ptr<GameObject>>& GetGameObjects() const;
  std::vector<std::shared_ptr<GameObject>>& GetGameObjects();

  QSize GetSize() const;

  const Cell_& GetCell(const QPoint&) const;
  Cell_& GetCell(const QPoint&);

  QPixmap GetPixmap() const;

  void AddSoldier();
  void AddTerrainObject();

 private:
  struct Cell_ {
    std::vector<std::shared_ptr<TerrainObject>> terrain_objects;
  };

  QSize size_;
  QPixmap picture_;
  std::vector<std::vector<Cell_>> cells_;
  std::vector<std::shared_ptr<Soldier>> soldiers_;
  std::vector<std::shared_ptr<GameObject>> game_objects_;

  QPixmap DrawWorld() const;
};

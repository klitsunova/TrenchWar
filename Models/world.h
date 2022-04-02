#pragma once

#include <QPainter>

#include <memory>
#include <stack>
#include <vector>

#include "GameObjects/soldier.h"
#include "GameObjects/terrain_object.h"

class World {
 private:
  struct cell_;

 public:
  World() = delete;
  explicit World(QSize);

  ~World() = default;

  const std::vector<std::shared_ptr<Soldier>>& GetSoldiers() const;
  std::vector<std::shared_ptr<Soldier>>& GetSoldiers();
  const std::vector<std::shared_ptr<GameObject>>& GetGameObjects() const;
  std::vector<std::shared_ptr<GameObject>>& GetGameObjects();
  int GetWidth() const;
  int GetHeight() const;
  QSize GetSize() const;
  const cell_& GetCell(const QPoint&) const;
  cell_& GetCell(const QPoint&);
  QPixmap GetPixmap() const;

  void AddSoldier();
  void AddTerrainObject();

 private:
  QSize size_;
  QPixmap picture_;
  struct cell_ {
    std::vector<std::shared_ptr<TerrainObject>> terrain_objects;
  };
  std::vector<std::vector<cell_>> cells_;
  std::vector<std::shared_ptr<Soldier>> soldiers_;
  std::vector<std::shared_ptr<GameObject>> game_objects_;

  QPixmap DrawWorld() const;
};

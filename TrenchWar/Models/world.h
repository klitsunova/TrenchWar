#pragma once

#include <QPainter>

#include <memory>
#include <queue>
#include <stack>
#include <vector>

#include "GameObjects/soldier.h"
#include "GameObjects/terrain_object.h"
#include "Tools/pixmap_loader.h"

class World {
 private:
  struct Cell;

 public:
  explicit World(QSize);

  ~World() = default;

  const std::vector<std::shared_ptr<Soldier>>& GetSoldiers() const;
  std::vector<std::shared_ptr<Soldier>>& GetSoldiers();

  const std::vector<std::shared_ptr<GameObject>>& GetGameObjects() const;
  std::vector<std::shared_ptr<GameObject>>& GetGameObjects();

  const QSize& GetSize() const;

  const Cell& GetCell(const QPoint&) const;
  Cell& GetCell(const QPoint&);

  const QPixmap& GetPixmap() const;

  void AddSoldier();
  void AddSoldier(const QPoint&, bool);
  void AddTerrainObject();

  void UpdateDistances();

  void MoveSoldiers();

 private:
  struct Cell {
    std::vector<std::shared_ptr<TerrainObject>> terrain_objects;

    bool used;
    int distance;
  };

  QSize size_;
  QPixmap picture_;
  std::vector<std::vector<Cell>> cells_;
  std::vector<std::shared_ptr<Soldier>> soldiers_;
  std::vector<std::shared_ptr<GameObject>> game_objects_;

  QPixmap DrawWorld() const;
};

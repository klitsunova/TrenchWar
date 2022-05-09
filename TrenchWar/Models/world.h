#pragma once

#include <QFile>
#include <QPainter>

#include <memory>
#include <stack>
#include <string>
#include <vector>

#include "GameObjects/soldier.h"
#include "GameObjects/terrain_object.h"
#include "Tools/pixmap_loader.h"

class World {
 private:
  struct Cell;

 public:
  explicit World(const QString& path);

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
  void AddTerrainObject();

 private:
  void LoadMap(const QString& path);

  struct Landscape {
    Landscape(const QColor& q_color, int speed);
    QColor color = Qt::white;
    int speed_characteristic{0};
  };

  struct Cell {
    std::vector<std::shared_ptr<TerrainObject>> terrain_objects;
    Landscape landscape = Landscape(Qt::white, 1);
  };

  QSize size_;
  QPixmap picture_;
  std::vector<std::vector<Cell>> cells_;
  std::vector<std::shared_ptr<Soldier>> soldiers_;
  std::vector<std::shared_ptr<GameObject>> game_objects_;

  QPixmap DrawWorld() const;
};

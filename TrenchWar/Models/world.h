#pragma once

#include <QFile>
#include <QPainter>

#include <algorithm>
#include <functional>
#include <memory>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>

#include "GameObjects/soldier.h"
#include "GameObjects/terrain_object.h"
#include "Models/GameObjects/bullet.h"
#include "Tools/pixmap_loader.h"
#include "helpers/rivals.h"
#include "helpers/sizes.h"

class World {
 private:
  struct Cell;

 public:
  explicit World(const QString& path);

  ~World() = default;

  const std::vector<std::shared_ptr<Soldier>>& GetDefenders() const;
  const std::vector<std::shared_ptr<Soldier>>& GetAttackers() const;
  const std::vector<std::shared_ptr<TerrainObject>>& GetTerrainObjects() const;
  const std::vector<std::shared_ptr<Bullet>>& GetBullets() const;

  const QSize& GetSize() const;

  const Cell& GetCell(const QPoint&) const;
  Cell& GetCell(const QPoint&);

  const QPixmap& GetPixmap() const;

  void AddSoldier(Rival);
  void AddSoldier(const QPoint&, Rival);
  void AddTerrainObject();
  // void AddBullet(const QPoint&, const QPoint&, Rival, int = 100);
  void AddBullet(const std::shared_ptr<Bullet>&);

  void Update();

  void MoveSoldiers();
  void MoveBullets();

  void MakeShots();

 private:
  struct Landscape {
    Landscape(const QColor& q_color, int speed);
    QColor color = Qt::white;
    int move_lag{0};
  };

  struct Cell {
    std::vector<std::shared_ptr<TerrainObject>> terrain_objects;
    Landscape landscape{Landscape(Qt::white, 0)};

    std::set<std::shared_ptr<Soldier>> attackers;
    std::set<std::shared_ptr<Soldier>> defenders;

    bool used;
    int64_t ground_distance;
  };

  QSize size_;
  QPixmap picture_;
  std::vector<std::vector<Cell>> cells_;
  std::vector<std::shared_ptr<Soldier>> defenders_;
  std::vector<std::shared_ptr<Soldier>> attackers_;
  std::vector<std::shared_ptr<Bullet>> bullets_;
  std::vector<std::shared_ptr<TerrainObject>> terrain_objects_;
  bool is_need_update_towers_{true};

  void LoadMap(const QString& path);

  QPixmap DrawWorld() const;

  void UpdateGroundDistances();

  void DamageArea(int, int, int, int);

  void MakeShot(std::shared_ptr<Soldier>&);
};

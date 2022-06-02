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
#include "GameObjects/tower.h"
#include "Models/GameObjects/bullet.h"
#include "Tools/pixmap_loader.h"
#include "helpers/sides.h"
#include "helpers/sizes.h"

class World {
 private:
  struct Cell;

 public:
  explicit World(const QString& path);

  ~World() = default;

  const std::vector<std::shared_ptr<Soldier>>& GetSoldiers() const;
  const std::vector<std::shared_ptr<Tower>>& GetTowers() const;
  const std::vector<std::shared_ptr<Bullet>>& GetBullets() const;

  const QSize& GetSize() const;

  const Cell& GetCell(const QPoint&) const;
  Cell& GetCell(const QPoint&);

  const QPixmap& GetPixmap();
  void Update();
  void TrenchUpdate();

  void AddSoldier(Side side);
  void AddSoldier(const QPoint& position, Side side);
  void AddTower();
  void AddTower(const QPoint& position);
  void AddBullet(const std::shared_ptr<Bullet>& bullet);

  void MoveSoldiers();
  void MoveBullets();

  void MakeShots();

  void FireTower();

 private:
  struct Landscape {
    Landscape(const QColor& q_color, int speed);
    QColor color = Qt::white;
    int move_lag{0};
  };

  struct Cell {
    Landscape landscape{Landscape(Qt::white, 0)};
    bool is_trench;
    std::set<std::shared_ptr<Soldier>> soldiers;
    bool used;
    int64_t ground_distance;
  };

  QSize size_;
  QPixmap picture_;
  std::vector<std::vector<Cell>> cells_;
  std::vector<std::shared_ptr<Soldier>> soldiers_;
  std::vector<std::shared_ptr<Bullet>> bullets_;
  std::vector<std::shared_ptr<Tower>> towers_;
  bool is_need_update_towers_{true};
  int used_bullets_{0};
  int used_soldiers_{0};

  void LoadMap(const QString& path);

  QPixmap DrawWorld() const;

  void UpdateGroundDistances();

  void DamageArea(int x, int y, int radius, int bullet_index);

  std::optional<std::shared_ptr<Soldier>> FindNearest(
      const std::shared_ptr<Soldier>& soldier) const;
};
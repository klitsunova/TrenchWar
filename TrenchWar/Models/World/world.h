#pragma once

#include <QFile>
#include <QPainter>

#include <algorithm>
#include <functional>
#include <limits>
#include <list>
#include <memory>
#include <mutex>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <thread>
#include <vector>

#include "Models/GameObjects/soldier.h"
#include "Models/GameObjects/tower.h"
#include "Models/GameObjects/bullet.h"
#include "Models/Tools/pixmap_loader.h"
#include "Models/World/Cell.h"
#include "helpers/enum_helpers.h"
#include "helpers/sizes.h"
#include "Models/World/Maps/GroundDistancesMap.h"
#include <QMediaPlayer>

class World : public QObject {
  Q_OBJECT

 public:
  explicit World(const QString& path, GameMode mode, Side side);

  ~World() = default;

  const std::list<std::shared_ptr<Soldier>>& GetSoldiers() const;
  const std::list<std::shared_ptr<Tower>>& GetTowers() const;
  const std::list<std::shared_ptr<Bullet>>& GetBullets() const;

  const QSize& GetSize() const;

  bool IsTrench(const QPoint& position);
  void MakeTrench(const QPoint& position);
  void RemoveTrench(const QPoint& position);

  const QPixmap& GetPixmap();

  void AddSoldier(Side side);
  void AddSoldier(Side side, const QPoint& position);
  void AddTower();
  void AddTower(const QPoint& position);
  void AddBullet(const std::shared_ptr<Bullet>& bullet);

  void LoadBotData(Side side);

  void MoveSoldiers();
  void MoveBullets();

  void MakeShots();

  void FireTower();

  void UpdateCountAttackers();
  int GetCountAttackers() const;
  int GetCountTowers() const;

 signals:
  void Shot();

 private:
  int count_attackers_{0};
  std::vector<std::vector<Cell>> cells_;
  std::list<std::shared_ptr<Soldier>> soldiers_;
  std::list<std::shared_ptr<Bullet>> bullets_;
  std::list<std::shared_ptr<Tower>> towers_;
  std::vector<QPoint> bot_soldier_buffer_;
  GroundDistancesMap distances_map_;

  void LoadMap(const QString& path, GameMode mode, Side side);

  int GetLag(const QPoint& position);
  int GetDistance(const QPoint& position);
  void PutSoldierToCell(const std::shared_ptr<Soldier>& soldier);
  void RemoveSoldierFromCell(const std::shared_ptr<Soldier>& soldier);

  void DamageArea(int x, int y, int radius, const std::shared_ptr<Bullet>&);

  std::optional<std::shared_ptr<Soldier>> FindNearest(
      const std::shared_ptr<Soldier>& soldier) const;
};

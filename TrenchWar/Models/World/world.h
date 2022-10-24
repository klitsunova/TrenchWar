#pragma once

#include <QFile>
#include <QPainter>

#include <algorithm>
#include <functional>
#include <limits>
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
#include <QMediaPlayer>

class World : public QObject {
  Q_OBJECT

 public:
  explicit World(const QString& path, GameMode mode, Side side);

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
  QSize size_;
  QPixmap picture_;
  std::vector<std::vector<Cell>> cells_;
  std::vector<std::vector<std::vector<int>>> distances_;
  std::mutex distances_mutex_;
  std::queue<std::thread> distance_loading_threads_;
  std::vector<std::shared_ptr<Soldier>> soldiers_;
  std::vector<std::shared_ptr<Bullet>> bullets_;
  std::vector<std::shared_ptr<Tower>> towers_;
  std::vector<QPoint> bot_soldier_buffer_;
  bool is_need_update_towers_{true};
  int dead_soldiers_{0};

  void LoadMap(const QString& path, GameMode mode, Side side);

  QPixmap DrawWorld() const;

  void GenerateNewDistances(const QPoint& pos);
  int GetLag(const QPoint& position);
  int GetDistance(const QPoint& position);

  void RemoveSoldierFromCell(const std::shared_ptr<Soldier>& soldier);
  void PutSoldierToCell(const std::shared_ptr<Soldier>& soldier);

  void DamageArea(int x, int y, int radius, int bullet_index);

  std::optional<std::shared_ptr<Soldier>> FindNearest(
      const std::shared_ptr<Soldier>& soldier) const;
};

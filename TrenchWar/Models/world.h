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

#include "GameObjects/soldier.h"
#include "GameObjects/tower.h"
#include "Models/GameObjects/bullet.h"
#include "Tools/pixmap_loader.h"
#include "helpers/enum_helpers.h"
#include "helpers/sizes.h"
#include <QMediaPlayer>

class World : public QObject {
  Q_OBJECT

 private:
  struct Cell;

 public:
  explicit World(const QString& path, GameMode mode, Side side);

  ~World() = default;

  const std::list<std::shared_ptr<Soldier>>& GetSoldiers() const;
  const std::list<std::shared_ptr<Tower>>& GetTowers() const;
  const std::list<std::shared_ptr<Bullet>>& GetBullets() const;

  const QSize& GetSize() const;

  const Cell& GetCell(const QPoint&) const;
  Cell& GetCell(const QPoint&);

  const QPixmap& GetPixmap();
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
  int count_attackers_;
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
  };

  QSize size_;
  QPixmap picture_;
  std::vector<std::vector<Cell>> cells_;
  std::list<std::vector<std::vector<int>>> distances_;
  std::mutex distances_mutex_;
  std::queue<std::thread> distance_loading_threads_;
  std::list<std::shared_ptr<Soldier>> soldiers_;
  std::list<std::shared_ptr<Bullet>> bullets_;
  std::list<std::shared_ptr<Tower>> towers_;
  std::vector<QPoint> bot_soldier_buffer_;
  bool is_need_update_towers_{true};

  void LoadMap(const QString& path, GameMode mode, Side side);

  QPixmap DrawWorld() const;

  void GenerateNewDistances(std::vector<std::vector<int>>& distances_map,
                            const QPoint& pos);

  void DamageArea(int x, int y, int radius, const std::shared_ptr<Bullet>&);

  std::optional<std::shared_ptr<Soldier>> FindNearest(
      const std::shared_ptr<Soldier>& soldier) const;
};

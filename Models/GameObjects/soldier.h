#pragma once

#include <QPainter>

#include "game_object.h"

class Soldier : public GameObject {
  using health_t = int;

 public:
  Soldier();
  explicit Soldier(const QPoint&);

  ~Soldier() override = default;

  health_t GetHitPoints() const;

  void MoveSoldier(QSize);

 private:
  health_t hit_points_ = 100;
};

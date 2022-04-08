#pragma once

#include <QPainter>

#include "game_object.h"

#include <memory>

class Soldier : public GameObject {
  using Health = int;

 public:
  explicit Soldier();
  explicit Soldier(const QPoint&);

  ~Soldier() override = default;

  Health GetHitPoints() const;

  void MoveSoldier(QSize);

 private:
  Health hit_points_ = 100;
};

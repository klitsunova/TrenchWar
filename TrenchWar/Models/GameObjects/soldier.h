#pragma once

#include <QPainter>

#include <memory>

#include "game_object.h"

class Soldier : public GameObject {
  using Health = int;

 public:
  Soldier();
  explicit Soldier(const QPoint&);

  ~Soldier() override = default;

  Health GetHitPoints() const;

  void MoveSoldier(QSize);

 private:
  Health hit_points_ = 100;
};

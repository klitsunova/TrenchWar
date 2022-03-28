#pragma once

#include "game_object.h"

class Soldier : public GameObject {
 public:
  Soldier() = default;

  ~Soldier() override = default;

  int GetHitPoints();
  QColor GetColor() override;

  void MoveSoldier();

 private:
  using health_t = int;
  health_t hit_points_ = 100;
  QColor color_ = Qt::red;
};

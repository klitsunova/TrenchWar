#pragma once

#include "game_object.h"

class Soldier : public GameObject {
  using health_t = int;

 public:
  Soldier() = default;

  ~Soldier() override = default;

  health_t GetHitPoints() const;
  QColor GetColor() override;

  void MoveSoldier();

 private:
  health_t hit_points_ = 100;
  QColor color_ = Qt::red;
};

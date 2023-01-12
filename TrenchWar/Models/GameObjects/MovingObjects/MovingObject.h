#pragma once

#include <algorithm>

#include "Models/GameObjects/game_object.h"
class MovingObject : public GameObject {
 public:
  MovingObject() = default;
  explicit MovingObject(Side side, const QPoint& position);

  int GetTimeLag() const;
  void AddTimeLag(int lag = 0);
  void MakeTick();

 private:
  int time_lag_{0};
};

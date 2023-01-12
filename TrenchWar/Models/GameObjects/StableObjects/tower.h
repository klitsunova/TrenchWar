#pragma once

#include <algorithm>
#include <memory>

#include "Models/GameObjects/game_object.h"

class Tower : public GameObject {
 public:
  explicit Tower(const QPoint& point, Side side = Side::kDefender);

  ~Tower() override = default;

 private:
};

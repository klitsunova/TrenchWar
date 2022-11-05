#pragma once

#include <algorithm>
#include <memory>

#include "game_object.h"

class Tower : public GameObject {
 public:
  explicit Tower(const QPoint& point);

  ~Tower() override = default;

 private:
};

#pragma once

#include <algorithm>
#include <memory>

#include "game_object.h"

class Tower : public GameObject {
 public:
  explicit Tower(const QPoint& point);

  void TakeDamage(int multiply);

  bool IsDestroyed() const;

  ~Tower() override = default;

 private:
  int health = 100;
};

#pragma once

#include <memory>

#include "game_object.h"

class Tower : public GameObject {
 public:
  Tower();
  explicit Tower(const QPoint& point);

  void TakeDamage(int multiply);

  bool IsDestroyed() const;

  ~Tower() override = default;

 private:
  int health = 100;
};

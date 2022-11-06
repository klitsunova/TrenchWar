#pragma once

#include <memory>
#include <set>

#include "Models/GameObjects/MovingObjects/GroundMovingObjects/soldier.h"
class Cell {
 public:
  explicit Cell() = default;

  void InsertSoldier(const std::shared_ptr<Soldier>& soldier);
  void EraseSoldier(const std::shared_ptr<Soldier>& soldier);
  const std::set<std::shared_ptr<Soldier>>& GetSoldiers();

 private:
  std::set<std::shared_ptr<Soldier>> objects_;
};

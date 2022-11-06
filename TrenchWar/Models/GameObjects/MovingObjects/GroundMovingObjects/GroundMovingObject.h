#pragma once

#include "Models/GameObjects/MovingObjects/MovingObject.h"
#include "Models/World/Maps/GroundDistancesMap.h"
#include "Models/GameObjects/MovingObjects/AirMovingObjects/bullet.h"
class GroundMovingObject : public MovingObject {
 public:
  explicit GroundMovingObject() = default;
  explicit GroundMovingObject(Side side, const QPoint& position);

  void Move(const GroundDistancesMap& distances_map);

 private:
};

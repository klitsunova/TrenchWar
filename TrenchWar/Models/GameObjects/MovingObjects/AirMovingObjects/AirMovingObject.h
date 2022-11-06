#pragma once

#include <QPoint>

#include "helpers/enum_helpers.h"
#include "Models/GameObjects/MovingObjects/MovingObject.h"
class AirMovingObject : public MovingObject {
 public:
  explicit AirMovingObject() = default;
  explicit AirMovingObject(Side side, const QPoint& from, const QPoint& to);

  void Move();

  // TODO(AZUAVCHIKOV) remove
  bool IsUsed() const;
  void MakeUsed();

 private:
  int moving_progress_{0};
  QPoint from_;
  QPoint to_;
  bool is_used_{false};
};

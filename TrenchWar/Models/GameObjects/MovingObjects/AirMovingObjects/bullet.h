#pragma once

#include <QPixmap>
#include <QPoint>

#include <memory>

#include "helpers/enum_helpers.h"
#include "helpers/sizes.h"
#include "Models/GameObjects/game_object.h"
#include "Models/Tools/pixmap_loader.h"
#include "Models/GameObjects/MovingObjects/AirMovingObjects/AirMovingObject.h"

class Bullet : public AirMovingObject {
 public:
  Bullet() = delete;
  Bullet(const QPoint&, const QPoint&, Side, int = 100);

  ~Bullet() = default;

  int GetDamage() const;

 private:
  int damage_;
};

#pragma once

#include <QPixmap>
#include <QPoint>

#include <memory>

#include "helpers/sizes.h"
#include "Models/Tools/pixmap_loader.h"
#include "Models/GameObjects/game_object.h"

class Bullet : public GameObject {
 public:
  Bullet() = delete;
  explicit Bullet(const QPoint&, const QPoint&, int damage = 1);

  ~Bullet() = default;

  const QPoint& GetPosition() const;
  const QPoint& GetFromPosition() const;
  const QPoint& GetToPosition() const;

  int GetDamage() const;

  void Move();

 private:
  int moving_progress_{0};
  QPoint from_;
  QPoint to_;

  int damage_;
};

#pragma once

#include <QPixmap>
#include <QPoint>

#include <memory>

#include "Models/Tools/pixmap_loader.h"
#include "Models/GameObjects/game_object.h"

class Bullet : public GameObject {
 public:
  Bullet() = delete;
  explicit Bullet(const QPoint&, const QPoint&, int damage = 1);

  ~Bullet() = default;

  const QPoint& GetPosition() const;
  const QPoint& GetFromPosition() const;
  QPoint& GetFromPosition();
  const QPoint& GetToPosition() const;
  QPoint& GetToPosition();

  int GetDamage();

  const QPixmap& GetPixmap() const;

  void Move();

 private:
  int moving_progress{0};
  QPoint from_;
  QPoint to_;

  int damage_;
};

#pragma once

#include <QPixmap>
#include <QPoint>

#include <memory>

#include "helpers/rivals.h"
#include "helpers/sizes.h"
#include "Models/Tools/pixmap_loader.h"
#include "Models/GameObjects/game_object.h"

class Bullet : public GameObject {
 public:
  Bullet() = delete;
  Bullet(const QPoint&, const QPoint&, Rival, int = 100);

  ~Bullet() = default;

  const QPoint& GetPosition() const;
  const QPoint& GetFromPosition() const;
  const QPoint& GetToPosition() const;

  int GetDamage() const;

  Rival GetSide() const;

  void Move();

  bool IsUsed() const;
  void MakeUsed();

 private:
  int moving_progress_{0};
  QPoint from_;
  QPoint to_;
  Rival side_;
  bool is_used_{false};
  int damage_;
};

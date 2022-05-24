#pragma once

#include <QPixmap>
#include <QPoint>

#include <memory>

#include "helpers/sizes.h"
#include "Models/Tools/pixmap_loader.h"
#include "Models/GameObjects/game_object.h"
#include "Models/GameObjects/soldier.h"

class Bullet : public GameObject {
 public:
  Bullet() = delete;
  Bullet(const QPoint&, const QPoint&, Soldier::Type, int damage = 100);

  ~Bullet() = default;

  const QPoint& GetPosition() const;
  const QPoint& GetFromPosition() const;
  const QPoint& GetToPosition() const;

  int GetDamage() const;

  Soldier::Type GetType() const;

  void Move();

  bool IsUsed() const;
  void MakeUsed();

 private:
  int moving_progress_{0};
  QPoint from_;
  QPoint to_;
  Soldier::Type type_;
  bool is_used_{false};
  int damage_;
};

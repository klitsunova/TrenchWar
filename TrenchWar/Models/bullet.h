#pragma once

#include <QPixmap>
#include <QPoint>

#include <memory>

#include "Models/Tools/pixmap_loader.h"
#include "equipment.h"

class Bullet : public Equipment {
 public:
  Bullet() = delete;
  explicit Bullet(const QPoint&, const QPoint&, int damage = 1);

  ~Bullet() = default;

  void SetPosition(const QPoint&);

  EquipmentType GetType() const override;

  const QPoint& GetPosition() const;
  const QPoint& GetFromPosition() const;
  QPoint& GetFromPosition();
  const QPoint& GetToPosition() const;
  QPoint& GetToPosition();

  int GetDamage();

  const QPixmap& GetPixmap() const;

  void Move();

 private:
  QPoint position_;
  int moving_progress{0};
  QPoint from_;
  QPoint to_;
  std::shared_ptr<QPixmap> picture_;

  int damage_;
};

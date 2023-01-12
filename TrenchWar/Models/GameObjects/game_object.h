#pragma once

#include <QColor>
#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <QPointer>

#include <algorithm>
#include <memory>
#include <utility>

#include "Models/Tools/pixmap_loader.h"
#include "Models/Tools/random_generator.h"
#include "helpers/enum_helpers.h"

class GameObject {
 public:
  GameObject() = default;
  explicit GameObject(Side side, const QPoint& position);

  virtual ~GameObject() = default;

  const QPoint& GetPosition() const;

  void SetPosition(const QPoint&);
  void SetRandomPosition(const QRect&);

  const QPixmap& GetPixmap() const;

  QSize GetSize() const;
  void SetSize(const QSize&);

  Side GetSide() const;

  void TakeDamage(int damage);
  bool IsDead() const;

  int GetHealth() const;

 protected:
  static constexpr int kStep{1};
  static constexpr QSize kStandardSize{QSize(45, 45)};

  QSize size_{kStandardSize};
  QPoint position_;
  std::shared_ptr<QPixmap> picture_;
  int health_ = 100;
  Side side_;
};

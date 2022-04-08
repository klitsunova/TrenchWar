#pragma once

#include <QColor>
#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <QPointer>

#include "Models/Tools/pixmap_loader.h"

#include <memory>
#include <utility>

class GameObject {
 public:
  explicit GameObject();
  explicit GameObject(const QPoint&);

  virtual ~GameObject() = default;

  const QPoint& GetPosition() const;

  void SetPosition(const QPoint&);
  void SetRandomPosition(const QSize&);

  void MoveLeft();
  void MoveRight();
  void MoveUp();
  void MoveDown();

  const QPixmap& GetPixmap() const;

  QSize GetSize() const;
  void SetSize(const QSize&);

 protected:
  static constexpr int kStep{1};
  static constexpr QSize kStandardSize{QSize(30, 30)};

  QSize size_{kStandardSize};
  QPoint position_;
  std::shared_ptr<QPixmap> picture_;
};

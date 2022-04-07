#pragma once

#include <QColor>
#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <QPointer>

#include <utility>

class GameObject {
 public:
  GameObject();
  explicit GameObject(const QPoint&);

  virtual ~GameObject() = default;

  QPoint GetPosition() const;

  void SetPosition(const QPoint&);
  void SetRandomPosition(const QSize&);

  void MoveLeft();
  void MoveRight();
  void MoveUp();
  void MoveDown();

  QPixmap GetPixmap() const;

  static constexpr QSize StandardSize{QSize(30, 30)};

 protected:
  static constexpr int step_{1};

  QPoint position_;
  QPixmap picture_;
};

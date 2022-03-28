#pragma once

#include <QColor>
#include <QPoint>
#include <QPointer>
#include <utility>

class GameObject {
 public:
  GameObject();
  explicit GameObject(const QPoint& position);

  virtual ~GameObject() = default;

  QPoint GetPosition() const;
  virtual QColor GetColor() = 0;

  void SetColor(QColor color);
  void SetPosition(const QPoint& position);
  void SetRandomPosition();

  void MoveLeft();
  void MoveRight();
  void MoveUp();
  void MoveDown();

 protected:
  QPoint position_;
  QColor color_ = Qt::white;
};

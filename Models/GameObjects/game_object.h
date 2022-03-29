#pragma once

#include <QColor>
#include <QPainter>
#include <QPoint>
#include <QPointer>

#include <utility>

class GameObject {
 public:
  GameObject();
  explicit GameObject(int, int);

  virtual ~GameObject() = default;

  int GetXPosition() const;
  int GetYPosition() const;
  virtual QColor GetColor() = 0;

  void SetColor(QColor color);
  void SetPosition(int, int);
  void SetRandomPosition(int, int);

  void MoveLeft();
  void MoveRight();
  void MoveUp();
  void MoveDown();

  virtual void DrawObject(QPainter*, const QRect&) const = 0;

 protected:
  int x_position_;
  int y_position_;
  QColor color_ = Qt::white;
};

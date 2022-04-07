#pragma once

#include <QColor>
#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <QPointer>

#include <utility>

class GameObject {
 public:
  GameObject(const QString&);
  explicit GameObject(const QPoint&, const QString&);

  virtual ~GameObject() = default;

  const QPoint& GetPosition() const;

  void SetPosition(const QPoint&);
  void SetRandomPosition(const QSize&);

  void MoveLeft();
  void MoveRight();
  void MoveUp();
  void MoveDown();

  QPixmap GetPixmap() const;

  QSize GetSize() const;
  void SetSize(const QSize&);

 protected:
  static constexpr int step_{1};
  static constexpr QSize StandardSize{QSize(30, 30)};

  QSize size_{StandardSize};
  QPoint position_;
  QPixmap picture_;
};

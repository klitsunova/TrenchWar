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

class GameObject {
 public:
  explicit GameObject(const QPoint&);

  virtual ~GameObject() = default;

  const QPoint& GetPosition() const;

  void SetPosition(const QPoint&);
  void SetRandomPosition(const QRect&);

  const QPixmap& GetPixmap() const;

  QSize GetSize() const;
  void SetSize(const QSize&);

  int GetTimeLag() const;
  void SetTimeLag(int lag = 0);
  void MakeTick();

 protected:
  static constexpr int kStep{1};
  static constexpr QSize kStandardSize{QSize(45, 45)};

  QSize size_{kStandardSize};
  QPoint position_;
  std::shared_ptr<QPixmap> picture_;
  int time_lag_{0};
};

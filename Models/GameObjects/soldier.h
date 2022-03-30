#pragma once

#include "game_object.h"
#include <QPainter>

class Soldier : public GameObject {
  using health_t = int;

 public:
  Soldier();
  Soldier(int, int);

  ~Soldier() override = default;

  health_t GetHitPoints() const;
  QColor GetColor() override;
  void MoveSoldier(int, int);
  void DrawObject(QPainter*, const QRect&) const final;
  void DrawObject(QPainter*, const QPoint&) const final;

 private:
  health_t hit_points_ = 100;
  QColor color_ = Qt::red;
};

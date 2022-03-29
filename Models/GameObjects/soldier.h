#pragma once

#include <QPainter>
#include "game_object.h"

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

 private:
  health_t hit_points_ = 100;
  QColor color_ = Qt::red;
};

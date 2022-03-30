#include "soldier.h"

Soldier::Soldier() : GameObject() {}
Soldier::Soldier(int x_position, int y_position)
    : GameObject(x_position, y_position) {}

Soldier::health_t Soldier::GetHitPoints() const {
  return hit_points_;
}

QColor Soldier::GetColor() {
  return color_;
}

void Soldier::MoveSoldier(int width, int height) {
  // TODO(klitsunova): soldier's movement along the route
  if (x_position_ > 0) {
    MoveLeft();
  } else {
    SetRandomPosition(width, height);
  }
}

void Soldier::DrawObject(QPainter* painter,
                         const QRect& bounding_rect) const {
  painter->save();
  painter->setBrush(color_);
  painter->setPen(QPen(color_, 0));
  painter->drawEllipse(bounding_rect);
  painter->restore();
}

void Soldier::DrawObject(QPainter* painter,
                         const QPoint& point) const {
  painter->save();
  painter->setBrush(color_);
  painter->setPen(QPen(color_, 0));
  // (Alex Azyavchikov): magic constant needs to be fixed
  painter->drawEllipse(point.x() - 4, point.y() - 4, 8, 8);
  painter->restore();
}

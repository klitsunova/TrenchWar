#include "terrain_object.h"

TerrainObject::TerrainObject() : GameObject() {}

TerrainObject::TerrainObject(int x_position, int y_position)
    : GameObject(x_position, y_position) {}

QColor TerrainObject::GetColor() {
  return color_;
}

void TerrainObject::DrawObject(QPainter* painter,
                               const QRect& bounding_rect) const {
  painter->save();
  painter->setBrush(color_);
  painter->setPen(QPen(color_, 0));
  painter->drawRect(bounding_rect);
  painter->restore();
}

void TerrainObject::DrawObject(QPainter* painter,
                               const QPoint& point) const {
  painter->save();
  painter->setBrush(color_);
  painter->setPen(QPen(color_, 0));
  // (Alex Azyavchikov): magic constant needs to be fixed
  painter->drawRect(point.x() - 4, point.y() - 4, 8, 8);
  painter->restore();
}

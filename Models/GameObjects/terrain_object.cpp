#include "terrain_object.h"

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
TerrainObject::TerrainObject(int x_position, int y_position) :
                                                               GameObject() {
  SetPosition(x_position, y_position);
}

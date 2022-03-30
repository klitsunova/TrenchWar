#pragma once

#include "game_object.h"

class TerrainObject : public GameObject {
 public:
  TerrainObject();
  TerrainObject(int, int);

  ~TerrainObject() override = default;

  QColor GetColor() override;
  void DrawObject(QPainter*, const QRect&) const final;
  void DrawObject(QPainter*, const QPoint&) const final;

 private:
  QColor color_ = Qt::black;
};

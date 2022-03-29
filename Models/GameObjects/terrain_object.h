#pragma once

#include "game_object.h"

class TerrainObject : public GameObject {
 public:
  TerrainObject(int, int);

  ~TerrainObject() override = default;

  QColor GetColor() override;
  void DrawObject(QPainter*, const QRect&) const final;

 private:
  QColor color_ = Qt::black;
};

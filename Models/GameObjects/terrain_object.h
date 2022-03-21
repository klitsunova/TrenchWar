#ifndef TERRAIN_OBJECT_H
#define TERRAIN_OBJECT_H

#include "game_object.h"

class TerrainObject : public GameObject {
 public:
  TerrainObject() = default;
  ~TerrainObject() = default;
  QColor GetColor() override;
 private:
  QColor color_ = Qt::black;
};

#endif // TERRAIN_OBJECT_H

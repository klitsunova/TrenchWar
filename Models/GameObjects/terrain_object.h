#ifndef MODELS_GAMEOBJECTS_TERRAIN_OBJECT_H
#define MODELS_GAMEOBJECTS_TERRAIN_OBJECT_H

#include "game_object.h"

class TerrainObject : public GameObject {
 public:
  TerrainObject() = default;
  ~TerrainObject() = default;
  QColor GetColor() override;
 private:
  QColor color_ = Qt::black;
};

#endif  // MODELS_GAMEOBJECTS_TERRAIN_OBJECT_H

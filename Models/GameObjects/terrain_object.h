#pragma once

#include "game_object.h"

class TerrainObject : public GameObject {
 public:
  TerrainObject() = default;

  ~TerrainObject() override = default;

  QColor GetColor() override;

 private:
  QColor color_ = Qt::black;
};

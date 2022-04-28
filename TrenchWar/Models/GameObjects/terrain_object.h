#pragma once

#include <memory>

#include "game_object.h"

class TerrainObject : public GameObject {
 public:
  TerrainObject();
  explicit TerrainObject(const QPoint&);

  ~TerrainObject() override = default;

 private:
};

#pragma once

#include "game_object.h"

#include <memory>

class TerrainObject : public GameObject {
 public:
  TerrainObject();
  explicit TerrainObject(const QPoint&);

  ~TerrainObject() override = default;

 private:
};

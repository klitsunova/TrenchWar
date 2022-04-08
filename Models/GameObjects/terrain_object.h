#pragma once

#include "game_object.h"

class TerrainObject : public GameObject {
 public:
  TerrainObject(const std::shared_ptr<PixmapLoader>&);
  explicit TerrainObject(const QPoint&, const std::shared_ptr<PixmapLoader>&);

  ~TerrainObject() override = default;

 private:
};

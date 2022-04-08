#pragma once

#include "game_object.h"

#include <memory>

class TerrainObject : public GameObject {
 public:
  explicit TerrainObject(const std::shared_ptr<PixmapLoader>&);
  explicit TerrainObject(const QPoint&, const std::shared_ptr<PixmapLoader>&);

  ~TerrainObject() override = default;

 private:
};

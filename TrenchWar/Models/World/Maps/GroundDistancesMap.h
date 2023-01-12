#pragma once

#include <list>
#include <memory>
#include <vector>

#include "Models/World/Maps/Landscape/Landscape.h"
#include "LandscapeMap.h"
#include "Models/World/Layer.h"

class GroundDistancesMap : public LandscapeMap {
 public:
  GroundDistancesMap();
  explicit GroundDistancesMap(
      const std::vector<std::vector<Landscape>>& landscapes);
  explicit GroundDistancesMap(std::vector<std::vector<Landscape>>&& landscapes);

  int GetDistance(const QPoint& position) const;

  void AddObject(const std::shared_ptr<GameObject>& object);

  void EraseDeadObjects();

 private:
  std::list<Layer> object_layers_;
};


#pragma once
#include <QPainter>

#include <algorithm>
#include <limits>
#include <list>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

#include "Models/World/Maps/Landscape/Landscape.h"
#include "helpers/sizes.h"
#include "Models/GameObjects/StableObjects/tower.h"
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


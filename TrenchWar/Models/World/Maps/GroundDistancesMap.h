#pragma once
#include <QPainter>

#include <limits>
#include <list>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "Models/World/Maps/Landscape/Landscape.h"
#include "helpers/sizes.h"
#include "Models/GameObjects/tower.h"
#include "LandscapeMap.h"
#include "Models/World/Layer.h"

class GroundDistancesMap : public LandscapeMap {
 public:
  explicit GroundDistancesMap() = default;
  explicit GroundDistancesMap(const std::vector<std::vector<Landscape>>& landscapes);
  explicit GroundDistancesMap(std::vector<std::vector<Landscape>>&& landscapes);

  int GetDistance(const QPoint& position) override;

  void AddObject(const std::shared_ptr<GameObject>& object);

  void EraseObject(const std::shared_ptr<GameObject>& object);

 private:
  std::list<Layer> object_layers_;
};


#include "GroundDistancesMap.h"

// this empty constructors are need for threads:
// may happen situation when LandscapeMap is deleted but using in Layers
// that's why we need to first delete Layers and then LandscapeMap
// Note: first is deleting those object who was last constructed
GroundDistancesMap::GroundDistancesMap()
    : LandscapeMap(), object_layers_() {}

GroundDistancesMap::GroundDistancesMap
    (const std::vector<std::vector<Landscape>>& landscapes)
    : LandscapeMap(landscapes), object_layers_() {}

GroundDistancesMap::GroundDistancesMap
    (std::vector<std::vector<Landscape>>&& landscapes)
    : LandscapeMap(landscapes), object_layers_() {}

int GroundDistancesMap::GetDistance(const QPoint& position) const {
  int result = std::numeric_limits<int>::max();
  for (auto& layer : object_layers_) {
    assert(!layer.GetObject()->IsDead());
    result = std::min(result, layer.GetDistance(position));
  }
  return result;
}

void GroundDistancesMap::AddObject(const std::shared_ptr<GameObject>& object) {
  object_layers_.emplace_back(object, *this);
}

void GroundDistancesMap::EraseDeadObjects() {
  std::erase_if(object_layers_, [](const Layer& layer) {
    return layer.GetObject()->IsDead();
  });
}


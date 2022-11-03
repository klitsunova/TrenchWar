#include "GroundDistancesMap.h"

GroundDistancesMap::GroundDistancesMap(const std::vector<std::vector<Landscape>>& landscapes)
    : LandscapeMap(landscapes) {}

GroundDistancesMap::GroundDistancesMap(std::vector<std::vector<Landscape>>&& landscapes)
    : LandscapeMap(landscapes) {}

int GroundDistancesMap::GetDistance(const QPoint& position) {
  int result = std::numeric_limits<int>::max();
  for (auto& layer : object_layers_) {
    // TODO(AZUAVCHIKOV) if object is dead - delete
    // if(layer.GetObject().)
    result = std::min(result, layer.GetDistance(position));
  }
  return result;
}

void GroundDistancesMap::AddObject(const std::shared_ptr<GameObject>& object) {
  object_layers_.emplace_back(object, *this);
}

void GroundDistancesMap::EraseObject(const std::shared_ptr<GameObject>& object) {
  std::erase_if(object_layers_, [&](const Layer& layer) {
    return layer.GetObject() == object;
  });
}


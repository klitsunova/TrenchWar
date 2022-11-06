#pragma once

#include <memory>
#include <vector>
#include <thread>
#include <limits>

#include "Models/GameObjects/game_object.h"
#include "Models/World/Maps/LandscapeMap.h"
class Layer {
 public:
  explicit Layer() = default;
  explicit Layer(const std::shared_ptr<GameObject>& object,
                 const LandscapeMap& landscape_map);
  ~Layer();

  const std::shared_ptr<GameObject>& GetObject() const;

  int GetDistance(const QPoint& pos) const;

 private:
  void DistanceGeneratingAlgorithm(const LandscapeMap& landscape_map);
  void FinishGeneratingDistances() const;

  std::vector<std::vector<int>> distances_;
  mutable std::optional<std::thread> distance_loading_thread_{std::nullopt};
  std::shared_ptr<GameObject> object_;
};

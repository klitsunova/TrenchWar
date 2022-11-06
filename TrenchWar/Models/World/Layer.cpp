#include <queue>
#include "Layer.h"

Layer::Layer(const std::shared_ptr<GameObject>& object,
             const LandscapeMap& landscape_map)
    : object_(object),
      distances_(landscape_map.GetSize().height(),
                 std::vector<int>(landscape_map.GetSize().width())) {
  // DistanceGeneratingAlgorithm(landscape_map);
  distance_loading_thread_.emplace(&Layer::DistanceGeneratingAlgorithm,
                                   this,
                                   std::ref(landscape_map));
}

Layer::~Layer() {
  FinishGeneratingDistances();
}

const std::shared_ptr<GameObject>& Layer::GetObject() const {
  return object_;
}

int Layer::GetDistance(const QPoint& pos) const {
  FinishGeneratingDistances();
  return distances_[pos.y()][pos.x()];
}

void Layer::DistanceGeneratingAlgorithm(const LandscapeMap& landscape_map) {
  for (auto& cells_colum : distances_) {
    for (auto& cell : cells_colum) {
      cell = std::numeric_limits<int32_t>::max();
    }
  }
  std::vector<std::vector<bool>>
      used_cells(landscape_map.GetSize().height(),
                 std::vector<bool>(landscape_map.GetSize().width()));

  auto distance = [&](const QPoint& pos) -> int& {
    return distances_[pos.y()][pos.x()];
  };

  auto cmp = [&](const QPoint& left, const QPoint& right) {
    return distance(left) > distance(right);
  };

  std::priority_queue<QPoint, std::vector<QPoint>, decltype(cmp)>
      latest_at_ground(cmp);
  distance(object_->GetPosition()) = 0;
  latest_at_ground.push(object_->GetPosition());

  auto push_if = [&](const QPoint& pos, int dist, bool condition = true) {
    if (!condition || used_cells[pos.y()][pos.x()]) {
      return;
    }
    if (distance(pos) > dist + landscape_map.GetLag(pos)) {
      distance(pos) = dist + landscape_map.GetLag(pos);
      latest_at_ground.push(pos);
    }
  };

  while (!latest_at_ground.empty()) {
    QPoint pos = latest_at_ground.top();
    int current_dist = distance(pos);
    QPoint dx = QPoint(1, 0);
    QPoint dy = QPoint(0, 1);

    // left neighbor
    push_if(pos - dx, current_dist, (pos.x() != 0));
    // push_if(QPoint(x - 1, y), current_dist, (x != 0));

    // right neighbor
    push_if(pos + dx, current_dist,
            (pos.x() != landscape_map.GetSize().width() - 1));
    // push_if(QPoint(x + 1, y), current_dist,
    //         (x != landscape_map.GetSize().width() - 1));

    // upper neighbor
    push_if(pos - dy, current_dist, (pos.y() != 0));
    // push_if(QPoint(x, y - 1), current_dist, (y != 0));

    // lower neighbor
    push_if(pos + dy, current_dist,
            (pos.y() != landscape_map.GetSize().height() - 1));
    // push_if(QPoint(x, y + 1), current_dist,
    //         (y != landscape_map.GetSize().height() - 1));

    used_cells[pos.y()][pos.x()] = true;
    latest_at_ground.pop();
  }
}

void Layer::FinishGeneratingDistances() const {
  if (distance_loading_thread_.has_value()) {
    distance_loading_thread_->join();
    distance_loading_thread_.reset();
  }
}

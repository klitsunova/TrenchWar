#pragma once

#include <QPainter>

#include "game_object.h"

class Soldier : public GameObject {
  using Health = int;

 public:
  Soldier(const std::shared_ptr<PixmapLoader>&);
  explicit Soldier(const QPoint&, const std::shared_ptr<PixmapLoader>&);

  ~Soldier() override = default;

  Health GetHitPoints() const;

  void MoveSoldier(QSize);

 private:
  Health hit_points_ = 100;
};

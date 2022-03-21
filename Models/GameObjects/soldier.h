#ifndef SOLDIER_H
#define SOLDIER_H

#include "game_object.h"

class Soldier : public GameObject {
 public:
  Soldier() = default;
  ~Soldier() = default;
  int GetHitPoints();
  QColor GetColor() override;

 private:
  int hit_points_ = 100;
  QColor color_ = Qt::red;
};

#endif // SOLDIER_H

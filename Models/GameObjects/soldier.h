#ifndef MODELS_GAMEOBJECTS_SOLDIER_H_
#define MODELS_GAMEOBJECTS_SOLDIER_H_

#include "game_object.h"

class Soldier : public GameObject {
 public:
  Soldier() = default;
  ~Soldier() = default;
  int GetHitPoints();
  QColor GetColor() override;
  void MoveSoldier();

 private:
  int hit_points_ = 100;
  QColor color_ = Qt::red;
};

#endif  // MODELS_GAMEOBJECTS_SOLDIER_H_

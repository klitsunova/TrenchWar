#ifndef VIEWS_VIEW_H_
#define VIEWS_VIEW_H_

#include <vector>
#include <QPainter>

#include "Models/GameObjects/game_object.h"

class View {
 public:
  View() = default;
  ~View() = default;
  void Update(QPainter* painter, const std::vector<GameObject*>& objects);

 private:
  int scale = 4;
};

#endif  // VIEWS_VIEW_H_

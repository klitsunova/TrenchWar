#pragma once

#include <QPainter>
#include <QPoint>
#include <memory>
#include <vector>

#include "Models/GameObjects/game_object.h"

class View {
 public:
  View() = default;

  ~View() = default;

  int GetScale();

  void SetScale(int);

  void Update(
      QPainter* painter,
      const std::vector<std::shared_ptr<GameObject>>& objects);

 private:
  int scale_;
};

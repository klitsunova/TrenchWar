#pragma once

#include <QPainter>
#include <QPixmap>
#include <QPoint>

#include <memory>
#include <vector>

#include "Models/GameObjects/game_object.h"
#include "Models/world.h"

class View {
 public:
  View() = default;

  ~View() = default;

  int GetScale() const;

  void SetScale(int scale);

  void Update(
      QPainter* painter,
      const std::vector<std::shared_ptr<GameObject>>& objects,
      const World&);

 private:
  int scale_;
};

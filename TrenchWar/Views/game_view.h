#pragma once

#include <QPainter>
#include <QPixmap>
#include <QPoint>

#include <memory>
#include <vector>

#include "Models/GameObjects/game_object.h"
#include "Models/world.h"

class GameView {
 public:
  explicit GameView(const std::shared_ptr<World>&);

  ~GameView() = default;

  int GetScale() const;

  void SetScale(int scale);

  void Update(
      QPainter* painter,
      const std::vector<std::shared_ptr<GameObject>>& objects);

 private:
  int scale_;
  std::shared_ptr<World> world_;
};

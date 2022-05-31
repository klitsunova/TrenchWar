#include "game_controller.h"

#include <memory>

GameController::GameController(
    QWidget* parent,
    const std::shared_ptr<World>& world) {
  setParent(parent);
  world_ = world;
}

void GameController::SetWorldObjects() {
  // temporary code
  world_->AddSoldier(QPoint(250, 250), Side::kAttacker);
  world_->AddSoldier(QPoint(300, 300), Side::kDefender);
}

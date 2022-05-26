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
  world_->AddSoldier(QPoint(5, 7), Side::kAttacker);
  world_->AddSoldier(QPoint(3, 3), Side::kDefender);
}

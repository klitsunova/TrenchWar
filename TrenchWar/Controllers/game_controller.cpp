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
  // world_->AddSoldier(QPoint(300, 300), Side::kAttacker);
  // world_->AddSoldier(QPoint(310, 310), Side::kDefender);
  world_->AddSoldier(QPoint(350, 350), Side::kAttacker);

  world_->AddSoldier(QPoint(300, 300), Side::kDefender);
  world_->AddSoldier(QPoint(300, 400), Side::kDefender);
  world_->AddSoldier(QPoint(400, 300), Side::kDefender);
  world_->AddSoldier(QPoint(400, 400), Side::kDefender);

  world_->AddSoldier(QPoint(200, 350), Side::kDefender);
  world_->AddSoldier(QPoint(350, 200), Side::kDefender);
  world_->AddSoldier(QPoint(500, 350), Side::kDefender);
  world_->AddSoldier(QPoint(350, 500), Side::kDefender);
  world_->AddSoldier(QPoint(200, 200), Side::kAttacker);
  world_->AddSoldier(QPoint(200, 500), Side::kAttacker);
  world_->AddSoldier(QPoint(500, 200), Side::kAttacker);
  world_->AddSoldier(QPoint(500, 500), Side::kAttacker);

}

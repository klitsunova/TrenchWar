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
  for (int i = 0; i < 500; ++i) {
    world_->AddSoldier(Side::kAttacker);
    world_->AddSoldier(Side::kDefender);
    world_->AddSoldier(Side::kDefender);
  }
}
void GameController::SetWeaponsParameters() {
  int k = std::min(world_->GetSize().width(), world_->GetSize().height());
  // weapons::kBulletRadius = (k * 5) / 800;
  //
  // weapons::kRifleRange = (k * 3600) / 800;
}

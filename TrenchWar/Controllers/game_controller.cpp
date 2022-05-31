#include "game_controller.h"

#include <memory>

GameController::GameController(
    QWidget* parent,
    const std::shared_ptr<World>& world) {
  setParent(parent);
  world_ = world;
}

void GameController::SetWorldObjects() {
  for (int i = 0; i < 50; ++i) {
    world_->AddSoldier(Side::kDefender);
    world_->AddSoldier(Side::kAttacker);
  }
  // temporary code
  // world_->AddSoldier(QPoint(200, 200), Soldier::Type::kDefender);
  // world_->AddSoldier(QPoint(400, 400), Soldier::Type::kDefender);
  // world_->AddSoldier(QPoint(600, 600), Soldier::Type::kDefender);
  world_->AddTerrainObject();
// =======
//   // temporary code
//   world_->AddSoldier(QPoint(5, 7), Side::kAttacker);
//   world_->AddSoldier(QPoint(3, 3), Side::kDefender);
// >>>>>>> dev
}

void GameController::UpdateEnemyObjects(const PlayerData& data) {
    for (auto& soldier: data.soldiers) {
      world_->AddSoldier(QPoint(soldier.GetPosition()), Side::kAttacker);
    }
}

PlayerData GameController::GetDataToSend() {
  PlayerData new_data;
  // new_data.soldiers = *(world_->GetDefendersData());
  return new_data;
}
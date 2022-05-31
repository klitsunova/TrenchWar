#include "game_controller.h"

#include <memory>

GameController::GameController(
    QWidget* parent,
    const std::shared_ptr<World>& world) {
  setParent(parent);
  world_ = world;
}

void GameController::SetWorldObjects(Side side) {
  for (int i = 0; i < 50; ++i) {
    if (side == Side::kAttacker) {
      world_->AddSoldier(Side::kAttacker);
    } else {
      world_->AddSoldier(Side::kDefender);
    }
  }
  world_->AddTerrainObject();
}

void GameController::UpdateAttackers(const GameData& data) {
  for (size_t i = 0; i < data.soldiers.size(); ++i) {
    QPoint position = QPoint(data.soldiers[i].x, data.soldiers[i].y);
    if (i < world_->GetAttackers().size()) {
      world_->GetAttackers()[i]->SetPosition(position);
      world_->GetAttackers()[i]->SetHitPoints(data.soldiers[i].hit_points);
    } else {
      world_->AddSoldier(position, Side::kAttacker);
    }
  }
  AddBullets(data);
}

void GameController::UpdateDefenders(const GameData& data) {
  for (size_t i = 0; i < data.soldiers.size(); ++i) {
    QPoint position = QPoint(data.soldiers[i].x, data.soldiers[i].y);
    if (i < world_->GetDefenders().size()) {
      world_->GetDefenders()[i]->SetPosition(position);
      world_->GetDefenders()[i]->SetHitPoints(data.soldiers[i].hit_points);
    } else {
      world_->AddSoldier(position, Side::kDefender);
    }
  }
  AddBullets(data);
}

GameData GameController::GetAttackersData() {
  GameData new_data;
  for (const auto& soldier: world_->GetAttackers()) {
    SoldierData data{soldier->GetPosition().x(),
                     soldier->GetPosition().y(),
                     soldier->GetHitPoints()};
    new_data.soldiers.push_back(data);
  }
  for (const auto& bullet: world_->GetBullets()) {
    // if (bullet->GetProgress() == 0 && bullet->GetSide() != Side::kDefender) {
      new_data.bullets.push_back(*bullet);
    // }
  }
  return new_data;
}

GameData GameController::GetDefendersData() {
  GameData new_data;
  for (const auto& soldier: world_->GetDefenders()) {
    SoldierData data{soldier->GetPosition().x(),
                     soldier->GetPosition().y(),
                     soldier->GetHitPoints()};
    new_data.soldiers.push_back(data);
  }
  for (const auto& bullet: world_->GetBullets()) {
    if (bullet->GetProgress() == 0 && bullet->GetSide() != Side::kAttacker) {
      new_data.bullets.push_back(*bullet);
    }
  }
  return new_data;
}

void GameController::AddBullets(const GameData& data) {
  for (auto& bullet: data.bullets) {
    world_->AddBullet(std::make_shared<Bullet>(bullet));
  }
}

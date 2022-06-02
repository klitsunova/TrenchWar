#include "game_controller.h"

#include <memory>

#include "helpers/styles.h"

GameController::GameController(
    QWidget* parent,
    const std::shared_ptr<World>& world) {
  setParent(parent);
  world_ = world;
}

void GameController::SetWorldObjects(Side side) {
  for (int i = 0; i < 500; ++i) {
    if (side == Side::kAttacker) {
      world_->AddSoldier(Side::kAttacker);
    } else {
      world_->AddSoldier(Side::kDefender);
      if (i < 30) {
        world_->AddTower();
      }
    }
  }
}

void GameController::UpdateAttackers(const GameData& data) {
  for (size_t i = 0; i < data.soldiers.size(); ++i) {
    QPoint position = QPoint(data.soldiers[i].x, data.soldiers[i].y);
      world_->AddSoldier(position, Side::kAttacker);
  }
}

void GameController::UpdateDefenders(const GameData& data) {
  for (size_t i = 0; i < data.soldiers.size(); ++i) {
    QPoint position = QPoint(data.soldiers[i].x, data.soldiers[i].y);
      world_->AddSoldier(position, Side::kDefender);
  }
  for (const auto& object: data.terrain_objects) {
    world_->AddTower(QPoint(object.first, object.second));
  }
  for (const auto& trench: data.trenches) {
    world_->GetCell(QPoint(trench.first, trench.second)).is_trench = true;
    world_->GetCell(QPoint(trench.first, trench.second)).landscape.color =
        colors::kTrenchColor;
  }
}

GameData GameController::GetAttackersData() {
  GameData new_data;
  for (const auto& soldier: world_->GetSoldiers()) {
    SoldierData data{soldier->GetPosition().x(),
                     soldier->GetPosition().y(),
                     soldier->GetHitPoints()};
    new_data.soldiers.push_back(data);
  }
  return new_data;
}

GameData GameController::GetDefendersData() {
  GameData new_data;
  for (const auto& soldier: world_->GetSoldiers()) {
    SoldierData data{soldier->GetPosition().x(),
                     soldier->GetPosition().y(),
                     soldier->GetHitPoints()};
    new_data.soldiers.push_back(data);
  }

  for (const auto& object: world_->GetTowers()) {
    new_data.terrain_objects.emplace_back(
        object->GetPosition().x(), object->GetPosition().y());
  }

  for (int i = 0; i < world_->GetSize().width(); ++i) {
    for (int j = 0; j < world_->GetSize().height(); ++j) {
      if (world_->GetCell(QPoint(i, j)).is_trench) {
        new_data.trenches.emplace_back(i, j);
      }
    }
  }

  return new_data;
}

void GameController::SetWeaponsParameters() {
  int k = std::min(world_->GetSize().width(), world_->GetSize().height());
  // weapons::kBulletRadius = (k * 5) / 800;
  //
  // weapons::kRifleRange = (k * 3600) / 800;
}

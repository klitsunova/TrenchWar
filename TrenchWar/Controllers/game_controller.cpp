#include "game_controller.h"

#include <memory>

GameController::GameController(
    QWidget* parent,
    const std::shared_ptr<World>& world) {
  setParent(parent);
  world_ = world;
  InitializationWeapon();
}

void GameController::InitializationWeapon() {
  constexpr int kRifle_damage = 40;
  constexpr int kRifle_range = 5;
  constexpr int kRifle_reload_time = 3;
  constexpr double kRifle_hit_chance = 60;
  constexpr int kRifle_count_ammo = 30;

  constexpr int kKnife_damage = 10;
  constexpr int kKnife_range = 1;
  constexpr int kKnife_reload_time = 2;
  constexpr double kKnife_hit_chance = 50;
  constexpr int kKnife_count_ammo = -1;

  weapons_.emplace_back(Weapon::WeaponType::Rifle, kRifle_damage,
                        kRifle_range, kRifle_reload_time,
                        kRifle_hit_chance, kRifle_count_ammo);

  weapons_.emplace_back(Weapon::WeaponType::Knife, kKnife_damage,
                        kKnife_range, kKnife_reload_time,
                        kKnife_hit_chance, -kKnife_count_ammo);
}

void GameController::SetWorldObjects() {
  for (int i = 0; i < 50; ++i) {
    world_->AddSoldier(Soldier::Type::kDefender);
  }
  // temporary code
  // world_->AddSoldier(QPoint(200, 200), Soldier::Type::kDefender);
  // world_->AddSoldier(QPoint(400, 400), Soldier::Type::kDefender);
  // world_->AddSoldier(QPoint(600, 600), Soldier::Type::kDefender);
  world_->AddTerrainObject();
}

void GameController::UpdateEnemyObjects(const PlayerData& data) {
    for (auto& soldier: data.soldiers) {
      world_->AddSoldier(QPoint(soldier.GetPosition()), Soldier::Type::kAttacker);
    }
}

PlayerData GameController::GetDataToSend() {
  PlayerData new_data;
  new_data.soldiers = *(world_->GetDefendersData());
  return new_data;
}
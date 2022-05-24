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
  for (int i = 0; i < 500; ++i) {
    world_->AddSoldier(Soldier::Type::kAttacker);
  }
  // temporary code
  // world_->AddSoldier(QPoint(1, 0), Soldier::Type::kDefender);
  // world_->AddSoldier(QPoint(1, 3), Soldier::Type::kDefender);
  // world_->AddSoldier(QPoint(1, 7), Soldier::Type::kDefender);
  world_->AddSoldier(QPoint(10, 10), Soldier::Type::kDefender);
  world_->AddSoldier(QPoint(200, 200), Soldier::Type::kDefender);
  world_->AddSoldier(QPoint(200, 400), Soldier::Type::kDefender);
  world_->AddSoldier(QPoint(350, 350), Soldier::Type::kDefender);
  world_->AddSoldier(QPoint(500, 500), Soldier::Type::kDefender);
  // world_->AddTerrainObject();
}

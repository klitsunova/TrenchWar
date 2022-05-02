#include "controller.h"

#include <memory>

Controller::Controller() {
  world_ = std::make_shared<World>("../Resources/Maps/map1.txt");
  view_ = std::make_unique<View>(world_);
  timer_ = std::make_unique<QBasicTimer>();
  // temporary code
  for (int i = 0; i < 1000; ++i) {
    world_->AddSoldier();
  }
  world_->AddTerrainObject();
  timer_->start(kTimerInterval, this);
  InitializationWeapon();
}

void Controller::paintEvent(QPaintEvent*) {
  QPainter qp(this);
  view_->Update(&qp, world_->GetGameObjects());
}

void Controller::timerEvent(QTimerEvent*) {
  for (const auto& soldier : world_->GetSoldiers()) {
    soldier->MoveSoldier(world_->GetSize());
  }
  update();
}

void Controller::InitializationWeapon() {
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

#include "game_controller.h"

#include <memory>

GameController::GameController(QWidget* parent) {
  setParent(parent);
  world_ = std::make_shared<World>(":././Resources/Maps/map1.txt");
  view_ = std::make_unique<GameView>(world_);
  timer_ = std::make_unique<QBasicTimer>();
  // temporary code
  for (int i = 0; i < 1; ++i) {
    world_->AddSoldier();
  }
  // world_->AddSoldier(QPoint(10, 10), true);
  world_->AddTerrainObject();
  StartTimer();
  InitializationWeapon();
}

void GameController::paintEvent(QPaintEvent*) {
  QPainter qp(this);
  view_->Update(&qp, world_->GetGameObjects());
}

void GameController::timerEvent(QTimerEvent*) {
  for (const auto& soldier : world_->GetSoldiers()) {
    soldier->MoveSoldier(world_->GetSize());
  }
  // world_->MoveSoldiers();
  // assert(false);
  update();
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

void GameController::StartTimer() {
  if (!timer_->isActive()) {
    timer_->start(kTimerInterval, this);
  }
}

void GameController::PauseTimer() {
  if (timer_->isActive()) {
    timer_->stop();
  }
}

void GameController::closeEvent(QCloseEvent* event) {
  event->ignore();
  Exit();
}

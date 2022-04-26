#include "controller.h"

#include <memory>

Controller::Controller() {
  world_ = std::make_shared<World>(kWorldSize);
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
  weapons_.emplace_back(Weapon::WeaponType::Rifle, 40, 5, 3, 60, 30);
  weapons_.emplace_back(Weapon::WeaponType::Knife, 10, 1, 2, 50, -1);
}

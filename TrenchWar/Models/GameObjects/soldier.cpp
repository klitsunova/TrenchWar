#include "soldier.h"

Soldier::Soldier(Rival side)
    : GameObject() {
  picture_ = PixmapLoader::GetSoldier();
  side_ = side;
  weapons_.emplace_back(Weapon(Weapon::WeaponType::Rifle));
}

Soldier::Soldier(const QPoint& point, Rival side)
    : GameObject(point) {
  picture_ = PixmapLoader::GetSoldier();
  side_ = side;
  weapons_.emplace_back(Weapon(Weapon::WeaponType::Rifle));
}

Soldier::Health Soldier::GetHitPoints() const {
  return hit_points_;
}

void Soldier::MoveSoldier(QSize size) {
  // TODO(klitsunova): soldier's movement along the route
  if (position_.x() - kStep > 0) {
    MoveLeft();
    picture_ = PixmapLoader::GetLSoldier();
  } else {
    SetRandomPosition(size);
  }
}

int Soldier::GetId() const {
  return id_;
}

Rival Soldier::GetSide() const {
  return side_;
}

int Soldier::GetVisibilityRange() const {
  return visibility_range_;
}

void Soldier::AddWeapon(const Weapon& weapon) {
  auto it =
      std::find_if(weapons_.begin(), weapons_.end(),
                   [&](const Weapon& item) {
                     return item.GetWeaponType() == weapon.GetWeaponType();
                   });
  if (it == weapons_.end()) {
    weapons_.push_back(weapon);
  }
}

void Soldier::AddAmmo(Weapon::WeaponType type, int count_ammo) {
  auto it = std::find_if(weapons_.begin(), weapons_.end(),
                         [&](const Weapon& item) {
                           return item.GetWeaponType() == type;
                         });
  if (it != weapons_.end()) {
    it->AddAmmo(count_ammo);
  }
}

std::optional<std::shared_ptr<Bullet>> Soldier::Fire(const QPoint& from,
                                                     const QPoint& to) {
  assert(!weapons_.empty());
  if (side_ == Rival::kDefender) {
    return weapons_[0].Fire(from, to, Rival::kAttacker);
  } else if (side_ == Rival::kAttacker) {
    return weapons_[0].Fire(from, to, Rival::kDefender);
  }
}

void Soldier::TakeDamage(int damage) {
  hit_points_ -= damage;
  if (hit_points_ <= 0) {
    hit_points_ = 0;
  }
}

bool Soldier::IsDead() const {
  return hit_points_ <= 0;
}

#include "soldier.h"

Soldier::Soldier()
    : GameObject() {
  picture_ = PixmapLoader::GetSoldier();
  defender_type_ = false;
}

Soldier::Soldier(const QPoint& point, bool type)
    : GameObject(point) {
  picture_ = PixmapLoader::GetSoldier();
  defender_type_ = type;
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

bool Soldier::IsDefender() const {
  return defender_type_;
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

void Soldier::TakeDamage(int damage) {
  hit_points_ -= damage;
  if (hit_points_ <= 0) {
    hit_points_ = 0;
  }
}

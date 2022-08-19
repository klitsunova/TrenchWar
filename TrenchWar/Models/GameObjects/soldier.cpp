#include "soldier.h"

Soldier::Soldier(Side side, const QPoint& point)
    : GameObject(point) {
  picture_ = PixmapLoader::GetSoldier();
  side_ = side;
  weapons_.emplace_back(Weapon(Weapon::WeaponType::Rifle));
}

Soldier::Health Soldier::GetHitPoints() const {
  return hit_points_;
}

Side Soldier::GetSide() const {
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
  return weapons_[0].Fire(from, to, side_);
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


void Soldier::SetHitPoints(int hit_points) {
  hit_points_ = hit_points;
}

int Soldier::GetTowerDamage() const {
  return weapons::kTowerDamage;
}

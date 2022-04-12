#include "soldier.h"

Soldier::Soldier() : GameObject() {
  picture_ = QPixmap(":Resources/Images/Soldier1.png");
}

Soldier::Soldier(const QPoint& point)
    : GameObject(point) {
  picture_ = QPixmap(":Resources/Images/Soldier1.png");
}

Soldier::health_t Soldier::GetHitPoints() const {
  return hit_points_;
}

void Soldier::MoveSoldier(QSize size) {
  // TODO(klitsunova): soldier's movement along the route
  if (position_.x() - step_ > 0) {
    MoveLeft();
  } else {
    SetRandomPosition(size);
  }
}

int Soldier::GetId() const {
  return id_;
}
int Soldier::GetVisibilityRange() const {
  return visibility_range_;
}
void Soldier::SetDied(bool isDied) {
  is_died_ = isDied;
}
bool Soldier::IsDied() const {
  return is_died_;
}
void Soldier::AddWeapon(Weapon::WeaponType type) {
  auto it = std::find_if(weapon_and_ammo_.begin(), weapon_and_ammo_.end(),
                         [&](const std::pair<Weapon, int>& item) {
    return item.first.GetWeaponType() == type;
  });
  if (it == weapon_and_ammo_.end()) {
    Weapon new_weapon(type);
    weapon_and_ammo_.push_back(std::make_pair(
        new_weapon,new_weapon.GetDefaultCountAmmo()));
  }
}
void Soldier::AddAmmo(Weapon::WeaponType type, int count_ammo) {
  auto it = std::find_if(weapon_and_ammo_.begin(), weapon_and_ammo_.end(),
                         [&](const std::pair<Weapon, int>& item) {
    return item.first.GetWeaponType() == type;
  });
  if (it != weapon_and_ammo_.end()) {
    int index = it - weapon_and_ammo_.begin();
    weapon_and_ammo_[index].second += count_ammo;
  }
}
void Soldier::TakeDamage(int damage) {
  if (damage >= hit_points_) {
    hit_points_ = 0;
    is_died_ = true;
  } else {
    hit_points_ -= damage;
  }
}

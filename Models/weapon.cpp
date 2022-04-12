#include "weapon.h"

Weapon::Weapon(Weapon::WeaponType type) {
  weapon_type_ = type;
  InitializationFromType();
}
int Weapon::GetDamage() const {
  return damage_;
}
int Weapon::GetRange() const {
  return range_;
}
int Weapon::GetReloadTime() const {
  return reload_time_;
}
double Weapon::GetHitChance() const {
  return hit_chance_;
}
Weapon::WeaponType Weapon::GetWeaponType() const {
  return weapon_type_;
}
int Weapon::GetDefaultCountAmmo() const {
  return default_count_ammo_;
}
void Weapon::InitializationFromType() {
  switch (weapon_type_) {
    case WeaponType::Knife:
      default_count_ammo_ = -1;
      damage_ = 10;
      range_ = 1;
      reload_time_ = 2;
      hit_chance_ = 50;
      break;
    case WeaponType::Rifle:
      default_count_ammo_ =  30;
      damage_ = 40;
      range_ = 5;
      reload_time_ = 3;
      hit_chance_ = 60;
      break;
  }
}

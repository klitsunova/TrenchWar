#include "weapon.h"

Weapon::Weapon(WeaponType type, int damage, int range, int reload_time, double hit_chance, int count_ammo) {
  weapon_type_ = type;
  count_ammo_ = count_ammo;
  damage_ = damage;
  range_ = range;
  reload_time_ = reload_time;
  hit_chance_ = hit_chance;
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

int Weapon::GetCountAmmo() const {
  return count_ammo_;
}

void Weapon::AddAmmo(int count) {
  count_ammo_ += count;
}

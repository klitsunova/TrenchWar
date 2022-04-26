#pragma once

#include "equipment.h"

class Weapon : public Equipment {
 public:
  enum class WeaponType {
    Knife,
    Rifle
  };

  EquipmentType GetType() const override {
    return EquipmentType::Weapon;
  }

  Weapon(WeaponType type, int damage, int range,
                  int reload_time, double hit_chance_, int count_ammo);

  int GetDamage() const;
  int GetRange() const;
  int GetReloadTime() const;
  double GetHitChance() const;
  int GetCountAmmo() const;
  void AddAmmo(int count);

  WeaponType GetWeaponType() const;

 private:
  void InitializationFromType();

  WeaponType weapon_type_;
  int count_ammo_;
  int damage_;
  int range_;
  int reload_time_;
  double hit_chance_;
};

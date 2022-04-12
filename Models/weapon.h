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

  explicit Weapon(WeaponType type);

  int GetDamage() const;
  int GetRange() const;
  int GetReloadTime() const;
  double GetHitChance() const;
  int GetDefaultCountAmmo() const;

  WeaponType GetWeaponType() const;

 private:
  void InitializationFromType();
  WeaponType weapon_type_;
  int default_count_ammo_;
  int damage_;
  int range_;
  int reload_time_;
  double hit_chance_;
};

#pragma once

#include "equipment.h"
#include "Models/GameObjects/bullet.h"
#include "helpers/rivals.h"
#include "helpers/weapons.h"

class Weapon : public Equipment {
 public:
  enum class WeaponType {
    Knife,
    Rifle
  };

  EquipmentType GetType() const override {
    return EquipmentType::Weapon;
  }

  Weapon() = delete;
  Weapon(Weapon::WeaponType, int = 10000);

  int GetDamage() const;
  int GetRange() const;
  int GetReloadTime() const;
  double GetHitChance() const;
  int GetCountAmmo() const;
  WeaponType GetWeaponType() const;

  void AddAmmo(int count);

  std::optional<std::shared_ptr<Bullet>> Fire(const QPoint&, const QPoint&, Rival);

 private:
  void InitializationFromType();

  WeaponType weapon_type_;
  int count_ammo_;
  int damage_;
  int range_;
  int reload_time_;
  double hit_chance_;
  int reload_lag_{0};
};

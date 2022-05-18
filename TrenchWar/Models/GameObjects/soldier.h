#pragma once

#include <QPainter>

#include <memory>
#include <utility>
#include <vector>

#include "Models/weapon.h"
#include "game_object.h"

class Soldier : public GameObject {
  using Health = int;

 public:
  enum class Type {
    kDefender,
    kAttacker
  };

  Soldier();
  explicit Soldier(const QPoint&, Type);

  ~Soldier() override = default;

  Health GetHitPoints() const;

  int GetId() const;
  Type GetType() const;
  int GetVisibilityRange() const;

  void AddWeapon(const Weapon& weapon);
  void AddAmmo(Weapon::WeaponType type, int count_ammo);

  void ConditionMonitoring() const;
  void ChooseWeapon() const;
  void Fire(int id, Weapon::WeaponType weapon_type);

  void TakeDamage(int damage);

 private:
  std::vector<Weapon> weapons_;
  int id_;
  int visibility_range_;
  Health hit_points_ = 100;
  Type type_;
};

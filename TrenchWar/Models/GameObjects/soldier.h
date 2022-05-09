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
  Soldier();
  explicit Soldier(const QPoint&, bool);

  ~Soldier() override = default;

  Health GetHitPoints() const;
  void MoveSoldier(QSize);

  int GetId() const;
  bool IsDefender() const;
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
  bool defender_type_;
};

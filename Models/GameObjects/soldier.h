#pragma once

#include <memory>

#include "game_object.h"
#include <Models/weapon.h>
#include <QPainter>
#include <utility>
#include <vector>

class Soldier : public GameObject {
  using Health = int;

 public:
  Soldier();
  explicit Soldier(const QPoint&);

  ~Soldier() override = default;

  Health GetHitPoints() const;
  void MoveSoldier(QSize);

  int GetId() const;
  int GetVisibilityRange() const;
  bool IsDied() const;

  void SetDied(bool);

  void AddWeapon(Weapon::WeaponType type);
  void AddAmmo(Weapon::WeaponType type, int count_ammo);

  void ConditionMonitoring() const;
  void ChooseWeapon() const;
  void Fire(int id, Weapon::WeaponType weapon_type);

  void TakeDamage(int damage);

 private:
  std::vector<std::pair<Weapon, int>> weapon_and_ammo_;
  int id_;
  int visibility_range_;
  bool is_died_;
  Health hit_points_ = 100;
};

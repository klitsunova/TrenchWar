#pragma once

#include <QPainter>

#include <memory>
#include <utility>
#include <vector>

#include "game_object.h"
#include "helpers/sides.h"
#include "Models/weapon.h"
#include "tower.h"

class Soldier : public GameObject {
  using Health = int;

 public:
  explicit Soldier(Side);
  explicit Soldier(const QPoint&, Side);

  ~Soldier() override = default;

  Health GetHitPoints() const;

  int GetId() const;
  Side GetSide() const;
  int GetVisibilityRange() const;

  int GetTowerDamage() const;

  void AddWeapon(const Weapon& weapon);
  void AddAmmo(Weapon::WeaponType type, int count_ammo);

  void ConditionMonitoring() const;
  void ChooseWeapon() const;
  void Fire(int id, Weapon::WeaponType weapon_type);
  std::optional<std::shared_ptr<Bullet>> Fire(const QPoint& from,
                                              const QPoint& to);

  void TakeDamage(int damage);

  bool IsDead() const;

 private:
  std::vector<Weapon> weapons_;
  int id_;
  int visibility_range_;
  Health hit_points_ = 100;
  Side side_;
};

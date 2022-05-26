#pragma once

#include <QPainter>

#include <memory>
#include <utility>
#include <vector>

#include "helpers/rivals.h"
#include "Models/weapon.h"
#include "game_object.h"

class Soldier : public GameObject {
  using Health = int;

 public:
  explicit Soldier(Rival);
  explicit Soldier(const QPoint&, Rival);

  ~Soldier() override = default;

  Health GetHitPoints() const;
  void MoveSoldier(QSize);

  int GetId() const;
  Rival GetSide() const;
  int GetVisibilityRange() const;

  void AddWeapon(const Weapon& weapon);
  void AddAmmo(Weapon::WeaponType type, int count_ammo);

  // void ConditionMonitoring() const;
  // void ChooseWeapon() const;
  // void Fire(int id, Weapon::WeaponType weapon_type);
  std::optional<std::shared_ptr<Bullet>> Fire(const QPoint&, const QPoint&);

  void TakeDamage(int damage);

  bool IsDead() const;

 private:
  std::vector<Weapon> weapons_;
  int id_;
  int visibility_range_;
  Health hit_points_ = 100;
  Rival side_;
};

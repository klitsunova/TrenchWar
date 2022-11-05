#pragma once

#include <QPainter>

#include <memory>
#include <utility>
#include <vector>

#include "Models/weapon.h"
#include "game_object.h"
#include "helpers/enum_helpers.h"
#include "tower.h"

class Soldier : public GameObject {
 public:
  explicit Soldier(Side, const QPoint&);

  ~Soldier() override = default;

  Side GetSide() const;
  int GetVisibilityRange() const;

  int GetTowerDamage() const;

  void AddWeapon(const Weapon& weapon);
  void AddAmmo(Weapon::WeaponType type, int count_ammo);

  std::optional<std::shared_ptr<Bullet>> Fire(const QPoint& from,
                                              const QPoint& to);

 private:
  std::vector<Weapon> weapons_;
  int visibility_range_;
  Side side_;
};

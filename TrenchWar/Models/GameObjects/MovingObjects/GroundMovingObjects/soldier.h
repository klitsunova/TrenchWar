#pragma once

#include <QPainter>

#include <memory>
#include <utility>
#include <vector>

#include "Models/weapon.h"
#include "Models/GameObjects/MovingObjects/GroundMovingObjects/GroundMovingObject.h"
#include "helpers/enum_helpers.h"
#include "Models/GameObjects/StableObjects/tower.h"

class Soldier : public GroundMovingObject {
 public:
  explicit Soldier(Side, const QPoint&);

  ~Soldier() override = default;

  int GetVisibilityRange() const;

  int GetTowerDamage() const;

  void AddWeapon(const Weapon& weapon);
  void AddAmmo(Weapon::WeaponType type, int count_ammo);

  std::optional<std::shared_ptr<Bullet>> Fire(const QPoint& from,
                                              const QPoint& to);

 private:
  std::vector<Weapon> weapons_;
  int visibility_range_;
};

#include "weapon.h"

#include "Models/Tools/MathHelper.h"

Weapon::Weapon(Weapon::WeaponType type, int count_ammo) {
  weapon_type_ = type;
  switch (type) {
    case WeaponType::Knife: {
      damage_ = weapons::kKnifeDamage;
      range_ = weapons::kKnifeRange;
      reload_time_ = weapons::kKnifeReloadTime;
      hit_chance_ = weapons::kKnifeHitChance;
      break;
    }
    case WeaponType::Rifle: {
      damage_ = weapons::kRifleDamage;
      range_ = weapons::kRifleRange;
      reload_time_ = weapons::kRifleReloadTime;
      hit_chance_ = weapons::kRifleHitChance;
      break;
    }
  }
  count_ammo_ = count_ammo;
}

int Weapon::GetDamage() const {
  return damage_;
}

int Weapon::GetRange() const {
  return range_;
}

int Weapon::GetReloadTime() const {
  return reload_time_;
}

double Weapon::GetHitChance() const {
  return hit_chance_;
}

Weapon::WeaponType Weapon::GetWeaponType() const {
  return weapon_type_;
}

int Weapon::GetCountAmmo() const {
  return count_ammo_;
}

void Weapon::AddAmmo(int count) {
  count_ammo_ += count;
}
std::optional<std::shared_ptr<Bullet>> Weapon::Fire(const QPoint& from,
                                                    const QPoint& to,
                                                    Side side) {
  if (reload_lag_ > 0) {
    --reload_lag_;
    return std::nullopt;
  }
  int dist = MathHelper::SquareDistance(from, to);
  if (dist > range_) {
    return std::nullopt;
  }
  reload_lag_ = reload_time_;
  return std::make_shared<Bullet>(from, to, side, damage_);
}

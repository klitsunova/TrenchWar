#pragma once

#include <QString>

namespace weapons {
const int kKnifeDamage = 100;
const int kKnifeRange = 1;  // 1*1
const int kKnifeReloadTime = 2;
const double kKnifeHitChance = 0.5;

// const int kRifleDamage = 40;
const int kRifleDamage = 110;
// const int kRifleRange = 3600;  // 75*75
const int kRifleRange = 100000000;  // 75*75
// const int kRifleReloadTime = 15;
const int kRifleReloadTime = 0;
const double kRifleHitChance = 0.6;

}  // namespace weapons

#pragma once
#include "equipment.h"

class Armor : public Equipment {
 public :
  EquipmentType GetType() const override {
    return EquipmentType::Armor;
  }
};

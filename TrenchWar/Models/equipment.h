#pragma once

class Equipment {
 public :
  enum class EquipmentType {
    Armor,
    Weapon
  };

  virtual EquipmentType GetType() const  = 0;
};

#include "Cell.h"

void Cell::InsertSoldier(const std::shared_ptr<Soldier>& object) {
  objects_.insert(object);
}

void Cell::EraseSoldier(const std::shared_ptr<Soldier>& object) {
  objects_.erase(object);
}

const std::set<std::shared_ptr<Soldier>>& Cell::GetSoldiers() {
  return objects_;
}

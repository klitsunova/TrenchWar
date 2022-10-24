#include "Cell.h"
Cell::Cell(const QColor& color, int move_lag) : Landscape(color, move_lag) {}

void Cell::InsertSoldier(const std::shared_ptr<Soldier>& soldier) {
  soldiers_.insert(soldier);
}

void Cell::EraseSoldier(const std::shared_ptr<Soldier>& soldier) {
  soldiers_.erase(soldier);
}

const std::set<std::shared_ptr<Soldier>>& Cell::GetSoldiers() const {
  return soldiers_;
}

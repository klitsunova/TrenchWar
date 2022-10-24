#pragma once

#include <memory>
#include <set>

#include "Models/GameObjects/soldier.h"
#include "Models/World/Landscape.h"
class Cell : public Landscape {
 public:
  explicit Cell(const QColor& color = Qt::white,
                int move_lag = 0);

  void InsertSoldier(const std::shared_ptr<Soldier>& soldier);
  void EraseSoldier(const std::shared_ptr<Soldier>& soldier);
  const std::set<std::shared_ptr<Soldier>>& GetSoldiers() const;

 private:
  std::set<std::shared_ptr<Soldier>> soldiers_;
};

#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include <QWidget>

#include "Models/GameObjects/soldier.h"
#include "Models/weapon.h"
#include "Models/world.h"

class GameController : public QWidget {
  Q_OBJECT

 public:
  explicit GameController(
      QWidget* parent = nullptr,
      const std::shared_ptr<World>& = nullptr);
  ~GameController() override = default;

  void SetWorldObjects();

  void SetWeaponsParameters();

 signals:
  void Exit();

 private:
  std::shared_ptr<World> world_;
};

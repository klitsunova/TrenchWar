#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include <QWidget>

#include "Models/GameObjects/MovingObjects/GroundMovingObjects/soldier.h"
#include "Models/weapon.h"
#include "Models/World/world.h"
#include "Network/helpers.h"
#include "trench_controller.h"

class GameController : public QWidget {
  Q_OBJECT

 public:
  explicit GameController(
      QWidget* parent = nullptr,
      const std::shared_ptr<World>& = nullptr);
  ~GameController() override = default;

  void SetWorldObjects(Side side);
  void UpdateDefenders(const GameData& data);
  void UpdateAttackers(const GameData& data);
  GameData GetAttackersData();
  GameData GetDefendersData();

  void SetWeaponsParameters();

 signals:
  void Exit();

 private:
  std::shared_ptr<World> world_;
};

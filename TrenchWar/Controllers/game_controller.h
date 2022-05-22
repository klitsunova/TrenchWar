#pragma once

#include <memory>
#include <vector>

#include <QWidget>

#include "Models/GameObjects/soldier.h"
#include "Models/weapon.h"
#include "Models/world.h"
#include "Network/helpers.h"

class GameController : public QWidget {
  Q_OBJECT

 public:
  explicit GameController(
      QWidget* parent = nullptr,
      const std::shared_ptr<World>& = nullptr);
  ~GameController() override = default;

  void SetWorldObjects();
  void UpdateEnemyObjects(const PlayerData& data);
  PlayerData GetDataToSend();

 signals:
  void Exit();

 private:
  void InitializationWeapon();

  std::vector<Weapon> weapons_;
  std::shared_ptr<World> world_;
};

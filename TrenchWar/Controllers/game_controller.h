#pragma once

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

 signals:
  void Exit();

 private:
  static constexpr int kTimerInterval{10};

  std::shared_ptr<World> world_;
  std::unique_ptr<GameView> view_;
  std::unique_ptr<QBasicTimer> timer_;

  void InitializationWeapon();

  std::vector<Weapon> weapons_;
};

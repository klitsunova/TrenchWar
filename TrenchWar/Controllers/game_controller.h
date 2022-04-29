#pragma once

#include <QBasicTimer>
#include <QWidget>

#include <memory>
#include <vector>

#include "Models/GameObjects/soldier.h"
#include "Models/GameObjects/terrain_object.h"
#include "Models/Tools/pixmap_loader.h"
#include "Models/weapon.h"
#include "Models/world.h"
#include "Views/game_view.h"

class GameController : public QWidget {
  Q_OBJECT
 public:
  GameController();

  ~GameController() override = default;

  void paintEvent(QPaintEvent*) override;
  void timerEvent(QTimerEvent*) override;

 private:
  static constexpr int kTimerInterval{50};
  static constexpr QSize kWorldSize{QSize(1000, 1000)};

  std::shared_ptr<World> world_;
  std::unique_ptr<GameView> view_;
  std::unique_ptr<QBasicTimer> timer_;

  void InitializationWeapon();

  std::vector<Weapon> weapons_;
};

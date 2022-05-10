#pragma once

#include <QBasicTimer>
#include <QCloseEvent>
#include <QStyle>
#include <QWidget>

#include <memory>
#include <vector>

#include "Models/GameObjects/soldier.h"
#include "Models/GameObjects/terrain_object.h"
#include "Models/Tools/pixmap_loader.h"
#include "Models/weapon.h"
#include "Models/world.h"
#include "Views/game_view.h"
#include "helpers/sizes.h"

class GameController : public QWidget {
  Q_OBJECT

 public:
  explicit GameController(QWidget* parent = nullptr);

  ~GameController() override = default;

  void paintEvent(QPaintEvent*) override;
  void timerEvent(QTimerEvent*) override;
  void SetStyle();

  void StartTimer();
  void PauseTimer();

  void closeEvent(QCloseEvent* event) override;

  void SetFullScreen(bool is_fullscreen);

 signals:
  void Exit();

 private:
  static constexpr int kTimerInterval{50};

  std::shared_ptr<World> world_;
  std::unique_ptr<GameView> view_;
  std::unique_ptr<QBasicTimer> timer_;

  void InitializationWeapon();

  std::vector<Weapon> weapons_;
};

#pragma once

#include <QKeyEvent>
#include <QShortcut>
#include <QWidget>

#include "Controllers/game_controller.h"
#include "Views/Game/game_view.h"

class EventsController : public QWidget {
  Q_OBJECT

 public:
  enum stage {
    preparation,
    active,
  };

  explicit EventsController(QWidget* parent = nullptr);
  ~EventsController() override = default;

  void Start();
  void HideGame();

  void StartTimer();
  void PauseTimer();
  void timerEvent(QTimerEvent*) override;

  stage GetGameStage() const;

 signals:
  void ShowPauseMenu();

 private:
  void ConnectUI();

  static constexpr int kTimerInterval{50};
  std::shared_ptr<World> world_;
  std::unique_ptr<GameView> view_;
  std::unique_ptr<QBasicTimer> timer_;
  std::unique_ptr<GameController> game_controller_;

  stage game_stage = preparation;
};

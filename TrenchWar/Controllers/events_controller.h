#pragma once

#include <memory>
#include <utility>
#include <vector>

#include <QBasicTimer>
#include <QKeyEvent>
#include <QShortcut>
#include <QWidget>

#include "Controllers/game_controller.h"
#include "Controllers/trench_controller.h"
#include "Views/Game/game_view.h"
#include "helpers/styles.h"

class EventsController : public QWidget {
  Q_OBJECT

 public:
  enum class Stage {
    kPreparation,
    kActive,
  };

  explicit EventsController(QWidget* parent = nullptr);
  ~EventsController() override = default;

  void Start();
  void HideGame();
  void SetFullScreen(bool is_fullscreen);

  void BuildTrench();
  void DeleteTrench();

  void StartTimer();
  void PauseTimer();
  void timerEvent(QTimerEvent*) override;

  Stage GetGameStage() const;

  void MapPressHandler(QMouseEvent* event);
  void MapReleaseHandler(QMouseEvent* event);

 signals:
  void ShowPauseMenu();

 private:
  static constexpr int kTimerInterval{500};

  void ConnectUI();

  std::shared_ptr<World> world_;
  std::unique_ptr<TrenchController> trench_controller_;
  std::unique_ptr<GameView> view_;
  std::unique_ptr<QBasicTimer> timer_;
  std::unique_ptr<GameController> game_controller_;

  Stage game_stage = Stage::kPreparation;
};

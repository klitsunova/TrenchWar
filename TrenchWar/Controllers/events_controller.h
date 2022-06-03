#pragma once

#include <memory>
#include <utility>
#include <vector>

#include <QAudioOutput>
#include <QBasicTimer>
#include <QKeyEvent>
#include <QMediaPlayer>
#include <QShortcut>
#include <QWidget>

#include "Controllers/game_controller.h"
#include "Controllers/trench_controller.h"
#include "Network/network_view.h"
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

  void StartPreparationStage();
  void SetPreparedStatus();
  void StartActiveStage();
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

  void Shot();

 signals:
  void ShowPauseMenu();
  void ReturnToMainMenu();
  void HideMainMenu();

 private:
  static constexpr int kTimerInterval{30};

  void ConnectUI();

  std::shared_ptr<World> world_;
  std::unique_ptr<TrenchController> trench_controller_;
  std::unique_ptr<GameView> view_;
  std::unique_ptr<QBasicTimer> timer_;
  std::unique_ptr<GameController> game_controller_;
  std::unique_ptr<NetworkView> network_view_;
  std::shared_ptr<NetworkController> network_controller_;
  QMediaPlayer* player_;

  Stage game_stage = Stage::kPreparation;
};

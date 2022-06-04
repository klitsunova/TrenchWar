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
#include "Network/network_view.h"
#include "Views/Game/game_end_window.h"
#include "Views/Game/game_view.h"
#include "helpers/enum_helpers.h"
#include "helpers/styles.h"

class EventsController : public QWidget {
  Q_OBJECT

 public:
  enum class Stage {
    kPreparation,
    kActive,
  };

  explicit EventsController(QWidget* parent = nullptr,
                            GameMode mode = GameMode::kNetwork);
  ~EventsController() override = default;

  void StartPreparationStage();
  void SetPreparedStatus();
  void StartActiveStage();

  void HideGame();
  void SetFullScreen(bool is_fullscreen);

  void ConfirmPurchase(BuyMode mode, QString name = "");
  void CancelPurchase(BuyMode mode, QString name = "");
  void ChangeMode(BuyMode mode);

  void StartTimer();
  void PauseTimer();
  void timerEvent(QTimerEvent*) override;

  Stage GetGameStage() const;

  void MapPressHandler(QMouseEvent* event);
  void MapReleaseHandler(QMouseEvent* event);
  void MapDoubleClickHandler(QMouseEvent* event);

  void Shot();
  void CheckGameEnding();

 signals:
  void ShowPauseMenu();
  void ReturnToMainMenu();
  void HideMainMenu();
  void PlayGameMusic();

 private:
  void CloseFinishWindow();

  static constexpr int kTimerInterval{30};

  void ConnectUI();

  GameMode game_mode_;
  std::shared_ptr<World> world_;
  std::unique_ptr<TrenchController> trench_controller_;
  std::unique_ptr<GameView> view_;
  std::unique_ptr<QBasicTimer> timer_;
  std::unique_ptr<GameController> game_controller_;
  std::unique_ptr<NetworkView> network_view_;
  std::shared_ptr<NetworkController> network_controller_;
  GameFinishWindow* game_finish_window_;

  Stage game_stage = Stage::kPreparation;
  BuyMode buy_mode_;
  Side player_side_;
};

#include "events_controller.h"

#include "Network/network_view.h"

#include <QMessageBox>

EventsController::EventsController(QWidget* parent) {
  setParent(parent);
  network_view_ = std::make_unique<NetworkView>(this);
  network_view_->show();
  connect(network_view_.get(),
          &NetworkView::StartGame,
          this,
          &EventsController::StartPreparationStage);
  connect(network_view_.get(),
          &NetworkView::ReturnToMainMenu,
          this,
          &EventsController::ReturnToMainMenu);
}

void EventsController::timerEvent(QTimerEvent*) {
  world_->MoveSoldiers();
  world_->MoveBullets();
  network_controller_->SetOwnData(game_controller_->GetDataToSend());
  network_controller_->SendData();
  game_controller_->UpdateEnemyObjects(network_controller_->GetEnemyData());
  view_->UpdateMap();
}

void EventsController::StartTimer() {
  if (!timer_->isActive()) {
    // TODO(AZYAVCHIKOV) temporary code
    timer_->start(kTimerInterval, this);
  }
}

void EventsController::PauseTimer() {
  if (timer_->isActive()) {
    timer_->stop();
  }
}

void EventsController::ConnectUI() {
  connect(view_.get(),
          &GameView::Close,
          this,
          &EventsController::ShowPauseMenu);
  connect(view_.get(),
          &GameView::StartGame,
          this,
          &EventsController::SetPreparedStatus);
  connect(network_controller_.get(),
          &NetworkController::GotSignalForActiveStage,
          this,
          &EventsController::StartActiveStage);
}

void EventsController::HideGame() {
  if (network_view_) {
    network_view_->hide();
  }
  if (view_) {
    view_->hide();
  }
}

void EventsController::StartPreparationStage() {
  network_view_->hide();
  network_controller_ = network_view_->GetNetworkController();
  world_ = std::make_shared<World>(":Resources/Maps/map2.txt");
  view_ = std::make_unique<GameView>(this, world_);
  timer_ = std::make_unique<QBasicTimer>();
  game_controller_ = std::make_unique<GameController>(this, world_);
  game_controller_->SetWorldObjects();
  ConnectUI();
  view_->show();
}

void EventsController::SetPreparedStatus() {
  network_view_->SetEndPreparationStatus();
}

void EventsController::StartActiveStage() {
  view_->HideReadyButton();
  game_stage = Stage::kActive;
  StartTimer();
}

EventsController::Stage EventsController::GetGameStage() const {
  return game_stage;
}

void EventsController::SetFullScreen(bool is_fullscreen) {
  view_->SetFullScreen(is_fullscreen);
}

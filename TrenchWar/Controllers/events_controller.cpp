#include "events_controller.h"

EventsController::EventsController(QWidget* parent) {
  setParent(parent);
  world_ = std::make_shared<World>(":Resources/Maps/map1.txt");
  view_ = std::make_unique<GameView>(this, world_);
  timer_ = std::make_unique<QBasicTimer>();
  game_controller_ = std::make_unique<GameController>(this, world_);
  game_controller_->SetWorldObjects();
  ConnectUI();
  view_->show();
}

void EventsController::timerEvent(QTimerEvent*) {
  for (const auto& soldier : world_->GetSoldiers()) {
    soldier->MoveSoldier(world_->GetSize());
  }
  view_->UpdateMap();
}

void EventsController::StartTimer() {
  if (!timer_->isActive()) {
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
          &EventsController::Start);
}

void EventsController::HideGame() {
  view_->hide();
}

void EventsController::Start() {
  view_->HideReadyButton();
  game_stage = Stage::kActive;
  StartTimer();
}

EventsController::Stage EventsController::GetGameStage() const {
  return game_stage;
}

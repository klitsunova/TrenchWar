#include "events_controller.h"

EventsController::EventsController(
    QWidget* parent) : game_controller_(new GameController()),
                       pause_(new QShortcut(Qt::Key_Escape, game_controller_)) {
  game_controller_->show();
  ConnectUI();
}

void EventsController::Pause() {
  game_controller_->PauseTimer();
}

void EventsController::Resume() {
  game_controller_->StartTimer();
}

void EventsController::ConnectUI() {
  connect(pause_,
          &QShortcut::activated,
          this,
          &EventsController::ShowPauseMenu);
  connect(game_controller_,
          &GameController::Exit,
          this,
          &EventsController::ShowPauseMenu);
}

void EventsController::HideGame() {
  game_controller_->hide();
}

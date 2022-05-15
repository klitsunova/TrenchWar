#include "main_controller.h"

MainController::MainController(QWidget* parent)
    : QWidget(parent),
      menu_controller_(new MenuController(this)),
      settings_(Settings::Instance()) {
  ConnectUI();
}

void MainController::ConnectUI() {
  connect(menu_controller_,
          &MenuController::StartGame,
          this,
          &MainController::StartGame);
  connect(menu_controller_,
          &MenuController::ResumeGame,
          this,
          &MainController::ResumeGame);
  connect(menu_controller_,
          &MenuController::BackToMenu,
          this,
          &MainController::ReturnToMenu);
  connect(menu_controller_,
          &MenuController::Exit,
          this,
          &MainController::Exit);
  connect(menu_controller_,
          &MenuController::MusicVolumeChanged,
          this,
          &MainController::ChangeMusic);
  connect(menu_controller_,
          &MenuController::FullScreenValueChanged,
          this,
          &MainController::ChangeScreenValue);
}

void MainController::StartGame() {
  menu_controller_->HideMenu();
  menu_controller_->SetGameStarted();
  events_controller_ = new EventsController(this);
  events_controller_->SetFullScreen(Settings::Instance()->IsFullScreen());
  ConnectEventsControllerUI();
}

void MainController::ReturnToMenu() {
  events_controller_->HideGame();
  delete events_controller_;
  events_controller_ = nullptr;
  menu_controller_->HidePauseMenu();
  menu_controller_->SetGameFinished();
  menu_controller_->ShowMenu();
}

void MainController::PauseGame() {
  events_controller_->PauseTimer();
  menu_controller_->ShowPauseMenu();
}

void MainController::ResumeGame() {
  if (events_controller_->GetGameStage() == EventsController::Stage::kActive) {
    events_controller_->StartTimer();
  }
  menu_controller_->HidePauseMenu();
}

void MainController::ConnectEventsControllerUI() {
  connect(events_controller_,
          &EventsController::ShowPauseMenu,
          this,
          &MainController::PauseGame);
}

void MainController::Exit() {
  QApplication::exit(0);
}


// TODO(Zolokinos)
void MainController::ChangeMusic() {}

void MainController::ChangeScreenValue() {
  if (events_controller_ != nullptr) {
    events_controller_->SetFullScreen(settings_->IsFullScreen());
  }
}

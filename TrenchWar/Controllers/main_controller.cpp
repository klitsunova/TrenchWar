#include "main_controller.h"

MainController::MainController(QWidget* parent)
    : QWidget(parent),
      menu_controller_(new MenuController(this)),
      settings_(Settings::getInstance()) {
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
  events_controller_->SetFullScreen(Settings::getInstance()->IsFullScreen());
  ConnectEventsControllerUI();
}

void MainController::ReturnToMenu() {
  events_controller_->HideGame();
  delete events_controller_;
  menu_controller_->HidePauseMenu();
  menu_controller_->SetGameFinished();
  menu_controller_->ShowMenu();
}

void MainController::PauseGame() {
  events_controller_->Pause();
  menu_controller_->ShowPauseMenu();
}

void MainController::ResumeGame() {
  events_controller_->Resume();
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

void MainController::ChangeMusic(bool is_game_started) {}
// TODO (Zolokinos)

void MainController::ChangeScreenValue(bool is_game_started) {
  if (is_game_started) {
    events_controller_->SetFullScreen(settings_->IsFullScreen());
  }
}

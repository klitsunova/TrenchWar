#include "main_controller.h"

MainController::MainController() : menu_controller_(new MenuController()) {
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
}

void MainController::StartGame() {
  menu_controller_->HideMenu();
  menu_controller_->SetGameStarted();
  game_controller_ = new GameController();
  game_controller_->show();
}

void MainController::ReturnToMenu() {
  delete game_controller_;
  menu_controller_->SetGameFinished();
  menu_controller_->ShowMenu();
}

void MainController::PauseGame() {
  game_controller_->PauseTimer();
  menu_controller_->ShowPauseMenu();
}

void MainController::ResumeGame() {
  game_controller_->StartTimer();
  menu_controller_->HidePauseMenu();
}

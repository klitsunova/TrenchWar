#include "main_controller.h"

MainController::MainController() : menu_controller_(new MenuController()) {
}

void MainController::StartGame() {
  game_controller_ = new GameController();
  game_controller_->show();
}

void MainController::ReturnToMenu() {
  delete game_controller_;
}

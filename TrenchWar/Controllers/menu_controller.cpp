#include "menu_controller.h"

MenuController::MenuController() : menu_(new MenuView()) {
  menu_->show();
}

void MenuController::paintEvent(QPaintEvent*) {
}

void MenuController::ShowMenu() {
  // menu_->show();
}

void MenuController::HideMenu() {
  // menu_->hide();
}

void MenuController::ShowPauseMenu() {
  // pause_menu_->show();
}

void MenuController::HidePauseMenu() {
  // pause_menu_->hide();
}

void MenuController::ShowSettingsMenu() {
  // settings_menu_view_->show();
}

void MenuController::HideSettingsMenu() {
  // settings_menu_view_->hide();
}

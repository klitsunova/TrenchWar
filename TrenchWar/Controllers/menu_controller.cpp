#include "menu_controller.h"

MenuController::MenuController() : menu_(new MenuView()),
                                   settings_menu_(new SettingsMenuView()),
                                   pause_menu_(new PauseMenuView()),
                                   exit_window_(new ExitWindow()) {
  ConnectUI();
  ShowMenu();
}

void MenuController::ConnectUI() {
  ConnectMenuUI();
  ConnectPauseMenuUI();
  ConnectSettingsUI();
  ConnectExitWindowUI();
}

void MenuController::ConnectMenuUI() {
  connect(menu_,
          &MenuView::StartButtonPressed,
          this,
          &MenuController::StartGame);
  connect(menu_,
          &MenuView::SettingsButtonPressed,
          this,
          &MenuController::ShowSettingsMenu);
  connect(menu_,
          &MenuView::ExitButtonPressed,
          this,
          &MenuController::ShowExitWindow);
}

void MenuController::ConnectPauseMenuUI() {
  connect(pause_menu_,
          &PauseMenuView::ResumeButtonPressed,
          this,
          &MenuController::ResumeGame);
  connect(pause_menu_,
          &PauseMenuView::BackButtonPressed,
          this,
          &MenuController::BackToMenu);
  connect(pause_menu_,
          &PauseMenuView::SettingsButtonPressed,
          this,
          &MenuController::ShowSettingsMenu);
  connect(pause_menu_,
          &PauseMenuView::ExitButtonPressed,
          this,
          &MenuController::ShowExitWindow);
}

void MenuController::ConnectSettingsUI() {
  connect(settings_menu_,
          &SettingsMenuView::ApplyChanges,
          this,
          &MenuController::SaveChanges);
  connect(settings_menu_,
          &SettingsMenuView::Cancel,
          this,
          &MenuController::HideSettingsMenu);
}

void MenuController::ConnectExitWindowUI() {
  connect(exit_window_,
          &ExitWindow::Exit,
          this,
          &MenuController::Exit);
  connect(exit_window_,
          &ExitWindow::Resume,
          this,
          &MenuController::HideExitWindow);
}

void MenuController::ShowMenu() {
  menu_->show();
}

void MenuController::HideMenu() {
  if (!is_game_started_) {
    menu_->hide();
  }
}

void MenuController::ShowPauseMenu() {
  pause_menu_->show();
}

void MenuController::HidePauseMenu() {
  if (is_game_started_) {
    pause_menu_->hide();
  }
}

void MenuController::ShowSettingsMenu() {
  if (is_game_started_) {
    HidePauseMenu();
  } else {
    HideMenu();
  }
  settings_menu_->show();
}

void MenuController::HideSettingsMenu() {
  settings_menu_->close();
  if (is_game_started_) {
    ShowPauseMenu();
  } else {
    ShowMenu();
  }
}

void MenuController::SetGameStarted() {
  is_game_started_ = true;
}

void MenuController::SetGameFinished() {
  is_game_started_ = false;
}

void MenuController::ShowExitWindow() {
  exit_window_->show();
}

void MenuController::HideExitWindow() {
  exit_window_->close();
}

void MenuController::SaveChanges() {
  // TODO(klitsunova): Save changes in Models/Tools/settings.h
  HideSettingsMenu();
}

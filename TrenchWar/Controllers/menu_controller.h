#pragma once

#include <QCloseEvent>
#include <QStyle>
#include <QWidget>

#include "Views/Menu/exit_window.h"
#include "Views/Menu/menu_view.h"
#include "Views/Menu/pause_menu_view.h"
#include "Views/Menu/settings_menu_view.h"

class MenuController : public QWidget {
  Q_OBJECT

 public:
  MenuController();
  ~MenuController() override = default;

  void ShowMenu();
  void HideMenu();

  void ShowPauseMenu();
  void HidePauseMenu();

  void SetGameStarted();
  void SetGameFinished();

  void ShowExitWindow();
  void HideExitWindow();

 signals:
  void StartGame();
  void ResumeGame();
  void BackToMenu();
  void Exit();

 private:
  void ConnectUI();
  void ConnectMenuUI();
  void ConnectPauseMenuUI();
  void ConnectSettingsUI();

  void ConnectExitWindowUI();
  void ShowSettingsMenu();
  void SaveChanges();

  void HideSettingsMenu();

  bool is_game_started_ = false;
  MenuView* menu_;
  SettingsMenuView* settings_menu_;
  PauseMenuView* pause_menu_;
  ExitWindow* exit_window_;
};

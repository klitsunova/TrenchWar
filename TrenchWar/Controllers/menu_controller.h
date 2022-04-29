#pragma once

#include <QWidget>

#include "Views/Menu/menu_view.h"
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

  void ShowSettingsMenu();
  void HideSettingsMenu();

  void paintEvent(QPaintEvent*) override;

  MenuView* menu_;
  SettingsMenuView* settings_menu_view_;
};

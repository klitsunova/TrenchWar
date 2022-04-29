#pragma once

#include <QWidget>

#include "Controllers/events_controller.h"
#include "Controllers/game_controller.h"
#include "Controllers/menu_controller.h"
#include "Models/Tools/settings.h"

class MainController : public QWidget {
  Q_OBJECT
 public:
  MainController();
  ~MainController() override = default;

  void StartGame();
  void ReturnToMenu();

  // void closeEvent(QCloseEvent* event) override;

 private:
  Settings* settings_;
  GameController* game_controller_ = nullptr;
  MenuController* menu_controller_;
  EventsController* events_controller_;

  QPixmap background_;
  bool is_game_started = false;
};

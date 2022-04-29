#pragma once

#include <QWidget>

#include "Controllers/events_controller.h"
#include "Controllers/menu_controller.h"
#include "Models/Tools/settings.h"

class MainController : public QWidget {
  Q_OBJECT

 public:
  MainController();
  ~MainController() override = default;

  void StartGame();
  void PauseGame();
  void ResumeGame();
  void ReturnToMenu();
  void Exit();

 private:
  void ConnectUI();
  void ConnectEventsControllerUI();

  MenuController* menu_controller_;
  EventsController* events_controller_ = nullptr;
  Settings* settings_;
};

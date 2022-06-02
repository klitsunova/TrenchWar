#pragma once

#include <QScreen>
#include <QWidget>

#include "Controllers/events_controller.h"
#include "Controllers/menu_controller.h"
#include "Models/Tools/settings.h"

class MainController : public QWidget {
  Q_OBJECT

 public:
  explicit MainController(QWidget* parent = nullptr);
  ~MainController() override = default;

  void StartNetworkGame();
  void StartBotGame();

  void PauseGame();
  void ResumeGame();
  void ReturnToMenu();
  void Exit();
  void ChangeMusic();
  void ChangeScreenValue();

 private:
  void ConnectUI();
  void ConnectEventsControllerUI();

  MenuController* menu_controller_;
  EventsController* events_controller_{nullptr};
  Settings* settings_{nullptr};
};

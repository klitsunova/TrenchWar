#pragma once

#include <QAudioOutput>
#include <QScreen>
#include <QMediaPlayer>
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
  void ChangeMusicVolume();
  void ChangeScreenValue();
  void HideMenu();

 private:
  void ConnectUI();
  void ConnectEventsControllerUI();

  void CreateAudioOutput();

  MenuController* menu_controller_;
  EventsController* events_controller_{nullptr};
  QMediaPlayer* music_player_;
};

#pragma once

#include <QKeyEvent>
#include <QShortcut>
#include <QWidget>

#include "Controllers/game_controller.h"

class EventsController : public QWidget {
  Q_OBJECT

 public:
  EventsController();
  ~EventsController() override = default;

  void Pause();
  void Resume();

  void HideGame();

 signals:
  void ShowPauseMenu();

 private:
  void ConnectUI();

  GameController* game_controller_;
  QShortcut* pause_;
};

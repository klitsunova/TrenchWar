#pragma once

#include <QKeyEvent>
#include <QShortcut>
#include <QWidget>

#include "Controllers/game_controller.h"

class EventsController : public QWidget {
  Q_OBJECT

 public:
  explicit EventsController(QWidget* parent = nullptr);
  ~EventsController() override = default;

  void Pause();
  void Resume();

  void HideGame();
  void SetFullScreen(bool is_fullscreen);

 signals:
  void ShowPauseMenu();

 private:
  void ConnectUI();

  GameController* game_controller_;
  QShortcut* pause_;
};

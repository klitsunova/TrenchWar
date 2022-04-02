#pragma once

#include <QBasicTimer>
#include <QWidget>

#include <memory>
#include <vector>

#include "Models/GameObjects/soldier.h"
#include "Models/GameObjects/terrain_object.h"
#include "Models/world.h"
#include "Views/view.h"

class Controller : public QWidget {
  Q_OBJECT
 public:
  Controller();

  ~Controller() override = default;

  void paintEvent(QPaintEvent*) override;
  void timerEvent(QTimerEvent*) override;

 private:
  std::unique_ptr<World> world_;
  std::unique_ptr<View> view_;
  std::unique_ptr<QBasicTimer> timer_;

  static constexpr int timer_interval_{50};
  static constexpr QSize world_size_{QSize(1000, 1000)};
};

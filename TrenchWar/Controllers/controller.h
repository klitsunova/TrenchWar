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
  static constexpr int kTimerInterval{50};
  static constexpr QSize kWorldSize{QSize(1000, 1000)};

  std::shared_ptr<World> world_;
  std::unique_ptr<View> view_;
  std::unique_ptr<QBasicTimer> timer_;
};
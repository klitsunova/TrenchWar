#ifndef CONTROLLERS_CONTROLLER_H_
#define CONTROLLERS_CONTROLLER_H_

#include <vector>
#include <QBasicTimer>
#include <QWidget>

#include "Views/view.h"
#include "Models/GameObjects/soldier.h"
#include "Models/GameObjects/terrain_object.h"

class Controller : public QWidget {
  Q_OBJECT
 public:
  Controller();
  ~Controller() override;
  std::vector<GameObject*> GetObjects() const;
  void paintEvent(QPaintEvent*) override;
  void timerEvent(QTimerEvent*) override;

 private:
  std::vector<GameObject*> objects_;
  Soldier* soldier_;
  TerrainObject* terrain_object_;
  View* view_;
  QBasicTimer* timer_;
};

#endif  // CONTROLLERS_CONTROLLER_H_

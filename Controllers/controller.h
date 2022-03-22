#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QWidget>

#include "Views/view.h"
#include "Models/GameObjects/soldier.h"
#include "Models/GameObjects/terrain_object.h"

class Controller : public QWidget {
  Q_OBJECT
 public:
  Controller();
  ~Controller() override;
  void Tick();
  std::vector<GameObject*> GetObjects() const;
  void paintEvent(QPaintEvent* event);

 private:
  std::vector<GameObject*> objects_;
  Soldier* soldier_;
  TerrainObject* terrain_object_;
  View* view_;
};

#endif // CONTROLLER_H

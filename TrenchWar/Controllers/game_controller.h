#pragma once

#include <QBasicTimer>
#include <QCloseEvent>
#include <QWidget>

#include <memory>
#include <vector>

#include "Models/GameObjects/soldier.h"
#include "Models/GameObjects/terrain_object.h"
#include "Models/Tools/pixmap_loader.h"
#include "Models/weapon.h"
#include "Models/world.h"
#include "Views/Game/map_view.h"
#include "helpers/sizes.h"

class GameController : public QWidget {
  Q_OBJECT

 public:
  explicit GameController(
      QWidget* parent = nullptr,
      const std::shared_ptr<World>& = nullptr);
  ~GameController() override = default;

  void SetWorldObjects();

 private:
  void InitializationWeapon();

  std::vector<Weapon> weapons_;
  std::shared_ptr<World> world_;
};

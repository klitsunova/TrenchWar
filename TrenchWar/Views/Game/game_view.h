#pragma once

#include <memory>

#include <QVBoxLayout>
#include <QWidget>
#include <QCloseEvent>
#include <QShortcut>

#include "Models/world.h"
#include "Views/Game/map_view.h"
#include "Views/Game/store_view.h"

class GameView : public QWidget {
  Q_OBJECT

 public:
  explicit GameView(
      QWidget* parent = nullptr,
      const std::shared_ptr<World>& world = nullptr);
  ~GameView() = default;

  void closeEvent(QCloseEvent* event) override;

  void UpdateMap();
  void HideReadyButton();

  void SetFullScreen(bool is_fullscreen);
  void SetStyle();

 signals:
  void StartGame();
  void Close();

 private:
  void ConnectUI();

  QVBoxLayout* layout_;
  MapView* map_;
  StoreView* store_;

  QShortcut* pause_;
};

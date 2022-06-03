#pragma once

#include <memory>

#include <QCloseEvent>
#include <QMessageBox>
#include <QShortcut>
#include <QVBoxLayout>
#include <QWidget>

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

  MapView* GetMap() const;
  StoreView* GetStore() const;

  void UpdateMap();
  void HideReadyButton();

  void SetFullScreen(bool is_fullscreen);
  void SetStyle();

  void SetWinState();
  void SetLoseState();
  void SetDrawState();

 signals:
  void StartGame();
  void Close();
  void GameFinishedEvent();

 private:
  void ConnectUI();

  QMessageBox* game_finished_message_;

  QPushButton* menu_button_;

  QVBoxLayout* layout_;
  MapView* map_;
  StoreView* store_;
  QShortcut* pause_;
};

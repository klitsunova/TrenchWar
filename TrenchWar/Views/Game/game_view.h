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
      const std::shared_ptr<World>& world = nullptr,
      std::function<void(QMouseEvent*)> mousePress = [](QMouseEvent*){},
      std::function<void(QMouseEvent*)> mouseMove = [](QMouseEvent*){},
      std::function<void(QMouseEvent*)> mouseRelease = [](QMouseEvent*){});
  ~GameView() = default;

  void closeEvent(QCloseEvent* event) override;

  MapView* GetMap() const;
  StoreView* GetStore() const;

  void UpdateMap();
  void HideReadyButton();

  void SetFullScreen(bool is_fullscreen);
  void SetStyle();

  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

 signals:
  void StartGame();
  void Close();

 private:
  std::function<void(QMouseEvent*)> mousePressEvent_;
  std::function<void(QMouseEvent*)> mouseMoveEvent_;
  std::function<void(QMouseEvent*)> mouseReleaseEvent_;

  void ConnectUI();

  QVBoxLayout* layout_;
  MapView* map_;
  StoreView* store_;
  QShortcut* pause_;
};

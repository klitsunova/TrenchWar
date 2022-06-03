#pragma once

#include <memory>

#include <QCloseEvent>
#include <QMessageBox>
#include <QShortcut>
#include <QVBoxLayout>
#include <QWidget>

#include "helpers/enum_helpers.h"
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
  void Start();
  void SetStoreDialog(QMouseEvent* event);

  void SetFullScreen(bool is_fullscreen);
  void SetStyle();

 signals:
  void StartGame(BuyMode mode);
  void Close();
  void ConfirmButtonPressed(BuyMode mode, QString name = "");
  void CancelButtonPressed(BuyMode mode, QString name = "");


 private:
  void ConnectUI();

  QVBoxLayout* layout_;
  MapView* map_;
  StoreView* store_;
  QShortcut* pause_;
};

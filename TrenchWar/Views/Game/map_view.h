#pragma once

#include <QMouseEvent>
#include <QWidget>

#include <memory>

#include "Models/World/world.h"
#include "Views/Game/buy_window.h"

class MapView : public QWidget {
  Q_OBJECT

 public:
  explicit MapView(QWidget* parent = nullptr,
                   const std::shared_ptr<World>& world = nullptr);
  ~MapView() = default;

  int GetScale() const;
  BuyWindow* GetBuyWindow();
  void SetScale(int scale);

  void SetStoreDialog(QMouseEvent* event);

  void SetObjectsVisibility(bool);

  void paintEvent(QPaintEvent* event) override;

  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void mouseDoubleClickEvent(QMouseEvent* event) override;

 signals:
  void MousePressedHandler(QMouseEvent* event);
  void MouseReleasedHandler(QMouseEvent* event);
  void MouseDoubleClickedHandler(QMouseEvent* event);
  void ConfirmButtonPressed(BuyMode mode, QString name = "");
  void CancelButtonPressed(BuyMode mode, QString name = "");

 private:
  void ConnectUI();

  int scale_;
  bool are_objects_visible_{true};
  std::shared_ptr<World> world_;
  BuyWindow* buy_window_;
};

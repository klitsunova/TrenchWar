#pragma once

#include <QMouseEvent>
#include <QWidget>

#include <memory>

#include "Models/world.h"
#include "Views/Game/buy_window.h"

class MapView : public QWidget {
  Q_OBJECT

 public:
  explicit MapView(QWidget* parent = nullptr,
                   const std::shared_ptr<World>& world = nullptr);
  ~MapView() = default;

  int GetScale() const;
  void SetScale(int scale);

  void DrawObject(QPainter* painter, const QPoint& pos,
                  const QSize& size, const QPixmap& picture);

  void SetStoreDialog(QMouseEvent* event);

  void paintEvent(QPaintEvent* event) override;

  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void mouseDoubleClickEvent(QMouseEvent* event) override;

 signals:
  void MousePressedHandler(QMouseEvent* event);
  void MouseReleasedHandler(QMouseEvent* event);
  void MouseDoubleClickedHandler(QMouseEvent* event);

 private:
  int scale_;
  std::shared_ptr<World> world_;
  BuyWindow* buy_window_;
};

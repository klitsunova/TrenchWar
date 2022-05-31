#pragma once

#include <QWidget>

#include <memory>

#include "Models/world.h"

class MapView : public QWidget {
  Q_OBJECT

 public:
  explicit MapView(QWidget* parent = nullptr,
                   const std::shared_ptr<World>& world = nullptr);
  ~MapView() = default;

  int GetScale() const;
  void SetScale(int scale);

  void DrawObject(QPainter& painter, const QPoint& pos,
                  const QSize& size, const QPixmap& picture);

  void paintEvent(QPaintEvent* event) override;

  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;



 signals:
  void MousePressedHandler(QMouseEvent *event);
  void MouseReleasedHandler(QMouseEvent *event);

 private:
  int scale_;
  std::shared_ptr<World> world_;
};

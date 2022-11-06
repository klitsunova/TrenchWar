#include "map_view.h"

#include <vector>

#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <QStyle>

#include "Models/GameObjects/game_object.h"
#include "helpers/sizes.h"
#include "helpers/styles.h"

MapView::MapView(QWidget* parent,
                 const std::shared_ptr<World>& world) {
  setParent(parent);
  // TODO(AZYAVCHIKOV): maybe not best solution
  // setMinimumSize(window_sizes::kWorld);
  buy_window_ = new BuyWindow();
  ConnectUI();
  world_ = world;
}

int MapView::GetScale() const {
  return scale_;
}

void MapView::SetScale(int scale) {
  scale_ = scale;
}

void MapView::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  painter.drawPixmap(this->rect(),
                     world_->GetPixmap(this->size(), are_objects_visible_));
}

void MapView::mousePressEvent(QMouseEvent* event) {
  MousePressedHandler(event);
}

void MapView::mouseReleaseEvent(QMouseEvent* event) {
  MouseReleasedHandler(event);
}

void MapView::mouseDoubleClickEvent(QMouseEvent* event) {
  MouseDoubleClickedHandler(event);
}

void MapView::SetStoreDialog(QMouseEvent* event) {
  buy_window_->show();
  QPoint position = event->globalPosition().toPoint();
  buy_window_->move(position);
  buy_window_->SetWindowLocation(position);
}

void MapView::SetObjectsVisibility(bool are_objects_visible) {
  are_objects_visible_ = are_objects_visible;
}

void MapView::ConnectUI() {
  connect(buy_window_,
          &BuyWindow::ConfirmButtonPressed,
          this,
          &MapView::ConfirmButtonPressed);
  connect(buy_window_,
          &BuyWindow::CancelButtonPressed,
          this,
          &MapView::CancelButtonPressed);
}

BuyWindow* MapView::GetBuyWindow() {
  return buy_window_;
}

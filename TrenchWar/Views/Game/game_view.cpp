#include "game_view.h"

#include <QLabel>
#include <QPainter>
#include <QStyle>
#include <utility>

#include "helpers/sizes.h"

GameView::GameView(QWidget* parent, const std::shared_ptr<World>& world,
    std::function<void(QMouseEvent*)> mousePress,
    std::function<void(QMouseEvent*)> mouseMove,
    std::function<void(QMouseEvent*)> mouseRelease)
    : layout_(new QVBoxLayout(this)),
      map_(new MapView(this, world)),
      store_(new StoreView(this)),
      pause_(new QShortcut(Qt::Key_Escape, this)) {
  layout_->addWidget(map_, 1);
  layout_->addWidget(store_, 0);
  SetStyle();
  ConnectUI();
  mousePressEvent_ = std::move(mousePress);
  mouseReleaseEvent_ = std::move(mouseRelease);
  mouseMoveEvent_ = std::move(mouseMove);
}

void GameView::ConnectUI() {
  connect(store_,
          &StoreView::Ready,
          this,
          &GameView::StartGame);
  connect(pause_,
          &QShortcut::activated,
          this,
          &GameView::Close);
}

void GameView::UpdateMap() {
  map_->update();
}

void GameView::closeEvent(QCloseEvent* event) {
  event->ignore();
  Close();
}

void GameView::HideReadyButton() {
  store_->HideReadyButton();
}

void GameView::SetFullScreen(bool is_fullscreen) {
  if (is_fullscreen) {
    showFullScreen();
  } else {
    showNormal();
  }
}

void GameView::SetStyle() {
  this->setGeometry(QStyle::alignedRect(
      Qt::LeftToRight,
      Qt::AlignCenter,
      window_sizes::kMenu,
      QApplication::primaryScreen()->availableGeometry()));
}

MapView* GameView::GetMap() const {
  return map_;
}

void GameView::mouseMoveEvent(QMouseEvent* event) {
  mouseMoveEvent_(event);
}

void GameView::mousePressEvent(QMouseEvent* event) {
  mousePressEvent_(event);
}

void GameView::mouseReleaseEvent(QMouseEvent* event) {
  mouseReleaseEvent_(event);
}

StoreView* GameView::GetStore() const {
  return store_;
}

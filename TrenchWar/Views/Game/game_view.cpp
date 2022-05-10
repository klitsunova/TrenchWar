#include "game_view.h"

#include <QLabel>
#include <QPainter>
#include <QStyle>

#include "helpers/sizes.h"

GameView::GameView(QWidget* parent, const std::shared_ptr<World>& world)
    : layout_(new QVBoxLayout(this)),
      map_(new MapView(this, world)),
      store_(new StoreView(this)),
      pause_(new QShortcut(Qt::Key_Escape, this)) {
  layout_->addWidget(map_);
  layout_->addWidget(store_);
  SetStyle();
  ConnectUI();
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

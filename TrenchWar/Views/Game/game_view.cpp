#include "game_view.h"

#include <QLabel>
#include <QStyle>

#include "helpers/sizes.h"

GameView::GameView(QWidget* parent, const std::shared_ptr<World>& world)
    : layout_(new QVBoxLayout(this)),
      map_(new MapView(this, world)),
      store_(new StoreView(this)),
      pause_(new QShortcut(Qt::Key_Escape, this)) {
  layout_->addWidget(map_, 1);
  layout_->addWidget(store_, 0);
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
  connect(store_,
          &StoreView::ConfirmButtonPressed,
          this,
          &GameView::ConfirmButtonPressed);
  connect(store_,
          &StoreView::CancelButtonPressed,
          this,
          &GameView::CancelButtonPressed);
  connect(map_,
          &MapView::ConfirmButtonPressed,
          this,
          &GameView::ConfirmButtonPressed);
  connect(map_,
          &MapView::CancelButtonPressed,
          this,
          &GameView::CancelButtonPressed);
}

void GameView::UpdateMap() {
  map_->update();
}

void GameView::closeEvent(QCloseEvent* event) {
  event->ignore();
  Close();
}

void GameView::Start() {
  store_->FixModes();
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

StoreView* GameView::GetStore() const {
  return store_;
}

void GameView::SetStoreDialog(QMouseEvent* event) {
  map_->SetStoreDialog(event);
}

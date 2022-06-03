#include "game_view.h"

#include <QLabel>
#include <QPainter>
#include <QStyle>

#include <utility>

#include "helpers/sizes.h"

GameView::GameView(QWidget* parent, const std::shared_ptr<World>& world)
    : layout_(new QVBoxLayout(this)),
      map_(new MapView(this, world)),
      store_(new StoreView(this)),
      pause_(new QShortcut(Qt::Key_Escape, this)),
      game_finished_message_(new QMessageBox(this)),
      menu_button_(new QPushButton(tr("menu"), this)){
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

StoreView* GameView::GetStore() const {
  return store_;
}

void GameView::SetWinState() {
  game_finished_message_->setWindowTitle("Trench War");
  game_finished_message_->setText("Win");
  game_finished_message_->addButton(menu_button_, QMessageBox::ActionRole);
  game_finished_message_->exec();
  if(game_finished_message_->clickedButton() == menu_button_) {
    GameFinishedEvent();
  }
}

void GameView::SetLoseState() {
  game_finished_message_->setWindowTitle("Trench War");
  game_finished_message_->setText("Lose");
  game_finished_message_->addButton(menu_button_, QMessageBox::ActionRole);
  game_finished_message_->exec();
  if(game_finished_message_->clickedButton() == menu_button_) {
    GameFinishedEvent();
  }
}

void GameView::SetDrawState() {
  game_finished_message_->setWindowTitle("Trench War");
  game_finished_message_->setText("Draw");
  game_finished_message_->addButton(menu_button_, QMessageBox::ActionRole);
  game_finished_message_->exec();
  if(game_finished_message_->clickedButton() == menu_button_) {
    GameFinishedEvent();
  }
}

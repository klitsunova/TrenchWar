#include "player_view.h"


PlayerView::PlayerView(QWidget* parent, const Player* player) : QWidget(parent),
                                                                player_(player),
                                                                nickname_(new QLabel(QString::number(player->GetId()), this)),
                                                                is_ready_(new QLabel(this)),
                                                                layout_(new QHBoxLayout(this)) {
  setAutoFillBackground(true);
  setPalette(QPalette(QColor(200, 200, 255)));
  // nickname_->setFont(fonts::);
  nickname_->setStyleSheet("QLabel {"
                           "font: bold 18px; }");
  // is_ready_->setFont(fonts::);
  is_ready_->setStyleSheet("QLabel {"
                           "font: bold 18px; }");
  if (player->IsReady()) {
    is_ready_->setText("Ready");
  } else {
    is_ready_->setText("Not ready");
  }
  layout_->addWidget(nickname_);
  layout_->addWidget(is_ready_);
}

void PlayerView::Highlight() {
  setAutoFillBackground(true);
  setPalette(QPalette(QColor(255, 200, 200)));
}

const Player* PlayerView::GetPlayer() const {
  return player_;
}

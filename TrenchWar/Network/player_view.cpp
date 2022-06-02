#include "player_view.h"

#include "helpers/fonts.h"
#include "helpers/styles.h"

PlayerView::PlayerView(QWidget* parent, const Player* player)
    : QWidget(parent),
      player_(player),
      nickname_(new QLabel(QString::number(player->GetId()), this)),
      is_ready_(new QLabel(this)),
      layout_(new QHBoxLayout(this)) {
  nickname_->setFont(fonts::kDialogLabel);
  is_ready_->setFont(fonts::kDialogLabel);
  if (player->IsReady()) {
    is_ready_->setText("Ready");
  } else {
    is_ready_->setText("Not ready");
  }
  layout_->addWidget(nickname_);
  layout_->addWidget(is_ready_);
}

void PlayerView::Highlight() {
  nickname_->setFont(fonts::kDialogLabelBold);
  is_ready_->setFont(fonts::kDialogLabelBold);
}

const Player* PlayerView::GetPlayer() const {
  return player_;
}

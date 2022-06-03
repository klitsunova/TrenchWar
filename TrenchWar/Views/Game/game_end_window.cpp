#include "game_end_window.h"

GameFinishWindow::GameFinishWindow(
    QWidget* parent) : layout_(new QGridLayout(this)),
                       text_label_(new QLabel(
                           "", this)),
                       menu_button_(new QPushButton("Menu", this)) {
  setParent(parent);
  setModal(true);
  setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
  SetStyles();
  SetLayout();
  ConnectUI();
}

void GameFinishWindow::SetStyles() {
  setMinimumSize(window_sizes::kDialogWidget);
  setStyleSheet(styles::kWidget);
  text_label_->setFont(fonts::kDialogLabel);

  menu_button_->setFont(fonts::kDialogButton);
  menu_button_->setMinimumSize(element_sizes::kDialogButton);
  menu_button_->setStyleSheet(styles::kPushButton);
}

void GameFinishWindow::SetLayout() {
  layout_->setSpacing(interval_sizes::kSpacing);
  layout_->addWidget(text_label_, 0, 0);
  auto* button_layout = new QHBoxLayout;
  button_layout->addWidget(menu_button_);
  layout_->addLayout(button_layout, 1, 0);
}

void GameFinishWindow::ConnectUI() {
  connect(menu_button_,
          &QPushButton::clicked,
          this,
          &GameFinishWindow::ToMenu);
}

void GameFinishWindow::Show(States state) {
  switch (state) {
    case States::kWin :
      text_label_->setText("You Win");
      text_label_->setStyleSheet("QLabel { color : green; }");
      break;
    case States::kLose:
      text_label_->setText("You Lose");
      text_label_->setStyleSheet("QLabel { color : red; }");
      break;
    case States::kDraw:
      text_label_->setText("Draw");
      break;
  }
  this->show();
}

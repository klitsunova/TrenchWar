#include "exit_window.h"

ExitWindow::ExitWindow() : layout_(new QGridLayout(this)),
                           question_label_(new QLabel("Are you sure you want to exit?", this)),
                           exit_button_(new QPushButton("Exit", this)),
                           cancel_button_(new QPushButton("Cancel", this)) {
  setModal(true);
  SetStyles();
  SetLayout();
  ConnectUI();
}

void ExitWindow::SetStyles() {
  setMinimumSize(window_sizes::kDialogSize);
  setStyleSheet(styles::kWidgetStyle);
  question_label_->setFont(fonts::kDialogTitleFont);
  exit_button_->setFont(fonts::kDialogButtonFont);
  exit_button_->setMinimumSize(element_sizes::kDialogButtonSize);
  exit_button_->setStyleSheet(styles::kExitButtonStyle);
  cancel_button_->setFont(fonts::kDialogButtonFont);
  cancel_button_->setMinimumSize(element_sizes::kDialogButtonSize);
  cancel_button_->setStyleSheet(styles::kPushButtonStyle);
}

void ExitWindow::SetLayout() {
  layout_->setSpacing(10);
  layout_->addWidget(question_label_, 0, 0);
  auto* button_layout = new QHBoxLayout;
  button_layout->addWidget(exit_button_);
  button_layout->addWidget(cancel_button_);
  layout_->addLayout(button_layout, 1, 0);
}

void ExitWindow::ConnectUI() {
  connect(exit_button_,
          &QPushButton::clicked,
          this,
          &ExitWindow::Exit);
  connect(cancel_button_,
          &QPushButton::clicked,
          this,
          &ExitWindow::Resume);
}

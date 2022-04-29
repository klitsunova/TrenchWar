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
  // TODO: Set styles
}

void ExitWindow::SetLayout() {
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

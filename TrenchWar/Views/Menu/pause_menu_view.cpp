#include "pause_menu_view.h"

PauseMenuView::PauseMenuView() : layout_(new QVBoxLayout(this)),
                                 resume_button_(new QPushButton(
                                     "Resume", this)),
                                 back_button_(new QPushButton(
                                     "Back to menu", this)),
                                 settings_button_(new QPushButton(
                                     "Settings", this)),
                                 exit_button_(new QPushButton(
                                     "Exit", this)) {
  setWindowTitle("Pause");
  SetStyles();
  SetLayout();
  ConnectUI();
}

void PauseMenuView::SetStyles() {
  setMinimumSize(window_sizes::kPauseMenu);
  setStyleSheet(styles::kWidget);
  for (auto& widget : children()) {
    auto* button_ptr = qobject_cast<QPushButton*>(widget);
    if (button_ptr != nullptr) {
      button_ptr->setFont(fonts::kMenuButton);
      button_ptr->setFixedSize(element_sizes::kMenuButton);
      button_ptr->setStyleSheet(styles::kPushButton);
    }
  }
}

void PauseMenuView::SetLayout() {
  layout_->setSpacing(10);
  layout_->addWidget(resume_button_);
  layout_->addWidget(back_button_);
  layout_->addWidget(settings_button_);
  layout_->addWidget(exit_button_);
  layout_->setAlignment(Qt::AlignCenter);
}

void PauseMenuView::ConnectUI() {
  connect(resume_button_,
          &QPushButton::clicked,
          this,
          &PauseMenuView::ResumeButtonPressed);
  connect(back_button_,
          &QPushButton::clicked,
          this,
          &PauseMenuView::BackButtonPressed);
  connect(settings_button_,
          &QPushButton::clicked,
          this,
          &PauseMenuView::SettingsButtonPressed);
  connect(exit_button_,
          &QPushButton::clicked,
          this,
          &PauseMenuView::ExitButtonPressed);
}

void PauseMenuView::closeEvent(QCloseEvent* event) {
  ResumeButtonPressed();
}

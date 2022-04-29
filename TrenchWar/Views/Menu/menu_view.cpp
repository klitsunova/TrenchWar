#include "menu_view.h"

MenuView::MenuView() : layout_(new QVBoxLayout(this)),
                       title_label_(new QLabel("TrenchWar", this)),
                       start_button_(new QPushButton("Start", this)),
                       settings_button_(new QPushButton("Settings", this)),
                       exit_button_(new QPushButton("Exit", this)) {
  setWindowTitle("Menu");
  SetStyles();
  SetLayout();
  ConnectUI();
}

void MenuView::SetStyles() {
}

void MenuView::SetLayout() {
  layout_->addWidget(title_label_);
  layout_->addWidget(start_button_);
  layout_->addWidget(settings_button_);
  layout_->addWidget(exit_button_);
}

void MenuView::ConnectUI() {
  connect(start_button_,
          &QPushButton::clicked,
          this,
          &MenuView::StartButtonPressed);
  connect(settings_button_,
          &QPushButton::clicked,
          this,
          &MenuView::SettingsButtonPressed);
  connect(exit_button_,
          &QPushButton::clicked,
          this,
          &MenuView::ExitButtonPressed);
}

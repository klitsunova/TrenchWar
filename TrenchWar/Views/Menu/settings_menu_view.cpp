#include "settings_menu_view.h"

SettingsMenuView::SettingsMenuView() : layout_(new QGridLayout(this)),
                                       apply_button_(new QPushButton("Apply", this)),
                                       cancel_button_(new QPushButton("Cancel", this)) {
  SetStyles();
  SetLayout();
  ConnectUI();
}

void SettingsMenuView::SetStyles() {
  // TODO: Set styles
}

void SettingsMenuView::SetLayout() {
  layout_->addWidget(apply_button_, 0, 0);
  layout_->addWidget(cancel_button_, 0, 1);
}

void SettingsMenuView::ConnectUI() {
  connect(apply_button_,
          &QPushButton::clicked,
          this,
          &SettingsMenuView::ApplyChanges);
  connect(cancel_button_,
          &QPushButton::clicked,
          this,
          &SettingsMenuView::Cancel);
}

void SettingsMenuView::closeEvent(QCloseEvent* event) {
  Cancel();
}

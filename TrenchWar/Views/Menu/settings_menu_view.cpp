#include "settings_menu_view.h"

SettingsMenuView::SettingsMenuView() : layout_(new QVBoxLayout(this)),
                                       settings_label_(new QLabel("Settings", this)),
                                       button_layout_(new QHBoxLayout()),
                                       apply_button_(new QPushButton("Apply", this)),
                                       cancel_button_(new QPushButton("Cancel", this)) {
  SetStyles();
  SetLayout();
  ConnectUI();
}

void SettingsMenuView::SetStyles() {
  setMinimumSize(window_sizes::kSettingsMenu);
  setStyleSheet(styles::kWidget);

  settings_label_->setFont(fonts::kTitleLabel);

  apply_button_->setFont(fonts::kDialogButton);
  apply_button_->setFixedSize(element_sizes::kDialogButton);
  apply_button_->setStyleSheet(styles::kPushButton);

  cancel_button_->setFont(fonts::kDialogButton);
  cancel_button_->setFixedSize(element_sizes::kDialogButton);
  cancel_button_->setStyleSheet(styles::kExitButton);
}

void SettingsMenuView::SetLayout() {
  layout_->addWidget(settings_label_, 1, Qt::AlignCenter);
  layout_->addStretch(10);
  button_layout_->addWidget(apply_button_);
  button_layout_->addWidget(cancel_button_);
  layout_->addLayout(button_layout_, Qt::AlignRight);
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

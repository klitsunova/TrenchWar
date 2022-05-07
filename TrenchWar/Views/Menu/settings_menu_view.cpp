#include "settings_menu_view.h"

SettingsMenuView::SettingsMenuView(
    QWidget* parent) : layout_(new QVBoxLayout(this)),
                       settings_label_(new QLabel("Settings", this)),
                       sound_label_(new QLabel("Sound: ", this)),
                       set_full_screen_label_(new QLabel("Set full screen: ", this)),
                       button_layout_(new QHBoxLayout()),
                       sound_layout_(new QHBoxLayout()),
                       set_full_screen_layout_(new QHBoxLayout()),
                       apply_button_(new QPushButton("Apply", this)),
                       cancel_button_(new QPushButton("Cancel", this)),
                       sound_slider_(new QSlider(this)),
                       set_full_screen_checkbox_(new QCheckBox()) {
  SetStyles();
  SetLayout();
  ConnectUI();
}

void SettingsMenuView::SetStyles() {
  setMinimumSize(window_sizes::kSettingsMenu);
  setStyleSheet(styles::kWidget);

  settings_label_->setFont(fonts::kTitleLabel);
  // settings_label_->setAlignment(Qt::AlignTop);
  sound_label_->setFont(fonts::kSettingsLabel);
  set_full_screen_label_->setFont(fonts::kSettingsLabel);
  // labels
  apply_button_->setFont(fonts::kDialogButton);
  apply_button_->setFixedSize(element_sizes::kDialogButton);
  apply_button_->setStyleSheet(styles::kPushButton);

  cancel_button_->setFont(fonts::kDialogButton);
  cancel_button_->setFixedSize(element_sizes::kDialogButton);
  cancel_button_->setStyleSheet(styles::kExitButton);
  // buttons
  sound_slider_->setOrientation(Qt::Horizontal);
  sound_slider_->setMinimum(0);
  sound_slider_->setMinimum(100);
}

void SettingsMenuView::SetLayout() {
  layout_->addWidget(settings_label_, 0, Qt::AlignCenter | Qt::AlignTop);
  // layout_->addStretch(interval_sizes::kStretch);

  sound_layout_->addWidget(sound_label_, 1);
  sound_layout_->addWidget(sound_slider_, 4);
  sound_layout_->addStretch(4);
  layout_->addLayout(sound_layout_, 1);

  set_full_screen_layout_->addWidget(set_full_screen_label_);
  set_full_screen_layout_->addWidget(set_full_screen_checkbox_);
  set_full_screen_layout_->addStretch(1);
  layout_->addLayout(set_full_screen_layout_, 1);
  layout_->addStretch(interval_sizes::kStretch);

  button_layout_->addWidget(apply_button_);
  button_layout_->addWidget(cancel_button_);
  layout_->addLayout(button_layout_);
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
  emit Cancel();
}

int SettingsMenuView::GetVolume() {
  return sound_slider_->sliderPosition();
}

bool SettingsMenuView::IsFullScreen() {
  return set_full_screen_checkbox_->isChecked();
}

#pragma once

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>

#include "helpers/fonts.h"
#include "helpers/sizes.h"
#include "helpers/styles.h"
#include "Models/Tools/settings.h"

class SettingsMenuView : public QWidget {
  Q_OBJECT

 public:
  explicit SettingsMenuView(QWidget* parent = nullptr);
  ~SettingsMenuView() override = default;
  int GetVolume();
  bool IsFullScreen();
  void ReturnToDefault();

 signals:
  void ApplyChanges();
  void Cancel();

 private:
  void SetStyles();
  void SetLayout();
  void ConnectUI();
  void SetValues();

  void closeEvent(QCloseEvent* event) override;

  QVBoxLayout* layout_;

  QLabel* settings_label_;
  QLabel* sound_label_;
  QLabel* set_full_screen_label_;

  QHBoxLayout* button_layout_;
  QHBoxLayout* sound_layout_;
  QHBoxLayout* set_full_screen_layout_;
  QPushButton* apply_button_;
  QPushButton* cancel_button_;
  QSlider* sound_slider_;
  QCheckBox* set_full_screen_checkbox_;

  Settings* settings_;
};

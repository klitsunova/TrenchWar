#pragma once

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "helpers/fonts.h"
#include "helpers/sizes.h"
#include "helpers/styles.h"

class SettingsMenuView : public QWidget {
  Q_OBJECT

 public:
  SettingsMenuView(QWidget* parent = nullptr);
  ~SettingsMenuView() override = default;

 signals:
  void ApplyChanges();
  void Cancel();

 private:
  void SetStyles();
  void SetLayout();
  void ConnectUI();

  void closeEvent(QCloseEvent* event) override;

  QVBoxLayout* layout_;
  QLabel* settings_label_;
  QHBoxLayout* button_layout_;
  QPushButton* apply_button_;
  QPushButton* cancel_button_;
};

#pragma once

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "Views/Menu/exit_window.h"

class MenuView : public QWidget {
  Q_OBJECT

 public:
  MenuView();
  ~MenuView() override = default;

 signals:
  void StartButtonPressed();
  void SettingsButtonPressed();
  void ExitButtonPressed();

 private:
  void SetStyles();
  void SetLayout();
  void ConnectUI();

  QVBoxLayout* layout_;
  QLabel* title_label_;
  QPushButton* start_button_;
  QPushButton* settings_button_;
  QPushButton* exit_button_;
  ExitWindow* exit_window_;
};

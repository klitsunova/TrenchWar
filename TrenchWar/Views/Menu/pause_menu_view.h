#pragma once

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "Views/Menu/exit_window.h"

class PauseMenuView : public QWidget {
  Q_OBJECT

 public:
  PauseMenuView();
  ~PauseMenuView() override = default;

 signals:
  void ResumeButtonPressed();
  void BackButtonPressed();
  void SettingsButtonPressed();
  void ExitButtonPressed();

 private:
  void SetStyles();
  void SetLayout();
  void ConnectUI();

  void closeEvent(QCloseEvent* event) override;

  QVBoxLayout* layout_;
  QPushButton* resume_button_;
  QPushButton* back_button_;
  QPushButton* settings_button_;
  QPushButton* exit_button_;
};

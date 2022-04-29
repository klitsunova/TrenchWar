#pragma once

#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

class ExitWindow : public QDialog {
  Q_OBJECT

 public:
  ExitWindow();
  ~ExitWindow() override = default;

 signals:
  void Resume();
  void Exit();

 private:
  void SetStyles();
  void SetLayout();
  void ConnectUI();

  QGridLayout* layout_;
  QLabel* question_label_;
  QPushButton* exit_button_;
  QPushButton* cancel_button_;
};

#pragma once

#include <QGridLayout>
#include <QPushButton>
#include <QWidget>

class SettingsMenuView : public QWidget {
  Q_OBJECT

 public:
  SettingsMenuView();
  ~SettingsMenuView() override = default;

 signals:
  void ApplyChanges();
  void Cancel();

 private:
  void SetStyles();
  void SetLayout();
  void ConnectUI();

  void closeEvent(QCloseEvent* event) override;

  QGridLayout* layout_;
  QPushButton* apply_button_;
  QPushButton* cancel_button_;
};

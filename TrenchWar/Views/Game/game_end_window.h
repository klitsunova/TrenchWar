#pragma once

#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "helpers/fonts.h"
#include "helpers/sizes.h"
#include "helpers/styles.h"

class GameFinishWindow : public QDialog {
  Q_OBJECT

 public:
  enum class States {
    kWin,
    kLose,
    kDraw
  };

  explicit GameFinishWindow(QWidget* parent = nullptr);
  ~GameFinishWindow() override = default;

  void Show(States state);

 signals:
  void ToMenu();

 private:
  void SetStyles();
  void SetLayout();
  void ConnectUI();

  QGridLayout* layout_;
  QLabel* text_label_;
  QPushButton* menu_button_;
};
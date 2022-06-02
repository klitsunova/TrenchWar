#pragma once

#include <vector>

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QWidget>

#include "helpers/modes.h"
#include "Models/Tools/pixmap_loader.h"
#include "Models/world.h"

class StoreView : public QWidget {
  Q_OBJECT

 public:
  explicit StoreView(QWidget* parent = nullptr);
  ~StoreView() = default;

  void HideReadyButton();
  void HideTrenchButtons() const;
  void ShowTrenchButtons() const;
  void paintEvent(QPaintEvent*) override;

 signals:
  void Ready();
  void ConfirmButtonPressed();
  void CancelButtonPressed();

 private:
  void AddItems();
  void SetMoneyWidget();
  void SetStyles();
  void ConnectUI();
  void SetNames();
  void SetLayout();

  QHBoxLayout* layout_;
  QPushButton* ready_button_;
  QPushButton* confirm_button_;
  QPushButton* cancel_button_;
  std::vector<QLabel*> items_;
  QButtonGroup* modes_;
  QWidget* money_widget_;
  QLabel* money_label_;
  QHBoxLayout* money_layout_;
  int count_money_{1000};
};

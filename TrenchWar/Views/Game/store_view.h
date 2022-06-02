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
  void EnableStoreButtons() const;
  void paintEvent(QPaintEvent*) override;
  void FixModes();

 signals:
  void Ready(BuyMode mode);
  void ConfirmButtonPressed(BuyMode mode);
  void CancelButtonPressed(BuyMode mode);
  void ModeChanged(BuyMode mode);

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
  QButtonGroup* purchase_modes_;
  QWidget* money_widget_area_;
  QLabel* money_label_;
  int count_money_{1000};
  BuyMode mode_{BuyMode::kTrench};
};

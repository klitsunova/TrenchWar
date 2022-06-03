#pragma once

#include <vector>

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QWidget>

#include "helpers/modes.h"
#include "helpers/store.h"
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
  bool SpendMoney(QString name);
  void ShowCost(int cost);
  void ClearToSpendMoneyLabel();

 signals:
  void Ready(BuyMode mode);
  void ConfirmButtonPressed(BuyMode mode, QString name = "");
  void CancelButtonPressed(BuyMode mode, QString name = "");
  void ModeChanged(BuyMode mode);

 private:
  void AddItems();
  void SetMoneyWidget();
  void SetToSpendMoneyWidget();
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
  QWidget* money_widget_area_to_spend_;
  QLabel* money_label_to_spend_;
  BuyMode mode_{BuyMode::kTrench};
  std::map<QString, int> price_list_;
};

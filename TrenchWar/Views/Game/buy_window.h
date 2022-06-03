#pragma once
#include <QDialog>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QTextEdit>
#include <map>

#include "helpers/enum_helpers.h"
#include "helpers/store.h"

class BuyWindow : public QDialog {
  Q_OBJECT

 public:
  explicit BuyWindow(QWidget* parent = nullptr);
  ~BuyWindow() override = default;

  void ClearWindow();
  void Close();
  void SetWindowLocation(QPoint point);
  QPoint GetLocation();

 signals:
  void ConfirmButtonPressed(BuyMode mode, QString name);
  void CancelButtonPressed(BuyMode mode, QString name = "");

 private:
  void SetStyles();
  void SetList();
  void SetLayout();
  void ConnectUI();
  void ChangeCost(QListWidgetItem* item);
  struct Data {
    QString filename;
    QString name;
    int cost;
  };

  QListWidget* list_;
  QLabel* total_price_;
  QPushButton* confirm_;
  QPushButton* cancel_;
  QTextEdit* count_;
  std::map<QString, int> price_list_;
  QPoint location_{};
};

#pragma once
#include <QDialog>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QTextEdit>

class BuyWindow : public QDialog {
  Q_OBJECT

 public:
  explicit BuyWindow(QWidget* parent = nullptr);
  ~BuyWindow() override = default;

 private:
  void SetStyles();
  void SetList();
  void SetLayout();
  QListWidget* list_;
  QLabel* total_price_;
  QPushButton* accept_;
  QPushButton* decline_;
  QTextEdit* count_;
};

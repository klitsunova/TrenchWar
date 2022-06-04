#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QPixmap>

#include "Models/Tools/pixmap_loader.h"
#include "buy_window.h"
#include "helpers/sizes.h"
#include "helpers/styles.h"

BuyWindow::BuyWindow(QWidget* parent)
    : list_(new QListWidget(this)),
      total_price_(new QLabel("0", this)),
      confirm_(new QPushButton("Confirm", this)),
      cancel_(new QPushButton("Cancel", this)),
      count_(new QTextEdit(this)) {
  // TODO(Zolokinos) Make area purchase
  count_->setVisible(false);
  confirm_->setDisabled(true);
  cancel_->setDisabled(true);
  setParent(parent);
  SetStyles();
  SetList();
  SetLayout();
  ConnectUI();
}

void BuyWindow::SetStyles() {
  list_->setIconSize(image_sizes::kIconStoreImage);
  setStyleSheet(styles::kStoreDialog);
  setFixedSize(window_sizes::kStoreDialog);
  setModal(true);
  setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
  confirm_->setStyleSheet(styles::kPushButton);
  cancel_->setStyleSheet(styles::kPushButton);
}

void BuyWindow::SetList() {
  auto* item = new QListWidgetItem;
  item->setText("Soldier");
  QPixmap pixmap(":Resources/Images/Soldier1.png");
  QIcon icon;
  icon.addPixmap(pixmap);
  item->setIcon(icon);
  list_->addItem(item);
  // TODO(Zolokinos) Tempo code
  price_list_["Soldier"] = 50;
}

void BuyWindow::SetLayout() {
  auto* h_box_layout = new QHBoxLayout(this);
  h_box_layout->addWidget(list_, 1);

  auto* v_box_layout = new QVBoxLayout();
  v_box_layout->addWidget(total_price_);
  v_box_layout->addWidget(confirm_);
  v_box_layout->addWidget(cancel_);
  v_box_layout->addStretch(1);

  h_box_layout->addLayout(v_box_layout);
}

void BuyWindow::ConnectUI() {
  connect(list_,
          &QListWidget::itemClicked,
          this,
          &BuyWindow::ChangeCost);
  connect(confirm_,
          &QPushButton::clicked,
          this, [&]() {
            emit ConfirmButtonPressed(
                BuyMode::kUnits,
                list_->currentItem()->text());
          });
  connect(cancel_,
          &QPushButton::clicked,
          this, [&]() {
            emit CancelButtonPressed(BuyMode::kUnits);
          });
}

void BuyWindow::ChangeCost(QListWidgetItem* item) {
  total_price_->setText(QString::number(price_list_[item->text()]));
  confirm_->setEnabled(true);
  cancel_->setEnabled(true);
}

void BuyWindow::ClearWindow() {
  list_->clearSelection();
  total_price_->setText(QString::number(0));
}

void BuyWindow::Close() {
  close();
}

void BuyWindow::SetWindowLocation(QPoint point) {
  location_ = point;
}

QPoint BuyWindow::GetLocation() {
  return location_;
}

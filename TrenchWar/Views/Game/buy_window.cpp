#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QLabel>

#include "buy_window.h"
#include "helpers/sizes.h"
#include "helpers/styles.h"
#include "Models/Tools/pixmap_loader.h"

BuyWindow::BuyWindow(QWidget* parent) :
    list_(new QListWidget(this)),
    total_price_(new QLabel("0", this)),
    accept_(new QPushButton("Accept", this)),
    decline_(new QPushButton("Decline", this)),
    count_(new QTextEdit(this)) {
  // TODO(Zolokinos) Make area purchase
  count_->setVisible(false);

  setParent(parent);
  SetStyles();
  SetList();
  SetLayout();
}

void BuyWindow::SetStyles() {
  list_->setIconSize(image_sizes::kIconStoreImage);
  setStyleSheet(styles::kStoreDialog);
  setFixedSize(window_sizes::kStoreDialog);
  setModal(true);
  setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
}

void BuyWindow::SetList() {
  // soldier
  QListWidgetItem* item = new QListWidgetItem;
  item->setText("Soldier");
  QPixmap pixmap =
      PixmapLoader::GetSoldier()->scaled(image_sizes::kIconStoreImage);
  QIcon icon;
  icon.addPixmap(pixmap);
  item->setIcon(icon);
  list_->addItem(item);
}

void BuyWindow::SetLayout() {
  auto* h_box_layout = new QHBoxLayout(this);
  h_box_layout->addWidget(list_, 1);

  auto* v_box_layout = new QVBoxLayout();
  v_box_layout->addWidget(total_price_);
  v_box_layout->addWidget(accept_);
  v_box_layout->addWidget(decline_);
  v_box_layout->addStretch(1);

  h_box_layout->addLayout(v_box_layout);
}

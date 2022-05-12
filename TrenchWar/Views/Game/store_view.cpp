#include "store_view.h"

#include <QLabel>

#include "helpers/sizes.h"
#include "helpers/styles.h"

StoreView::StoreView(
    QWidget* parent) : layout_(new QHBoxLayout(this)),
                       ready_button_(new QPushButton("Ready", this)) {
  setParent(parent);
  AddItems();
  SetStyles();
  ConnectUI();
}

void StoreView::HideReadyButton() {
  ready_button_->hide();
}

void StoreView::AddItems() {
  // temporary code
  for (int i = 0; i < 5; ++i) {
    auto* soldier = new QLabel;
    soldier->setPixmap(
        PixmapLoader::GetSoldier()->scaled(element_sizes::kStoreBox));
    items_.push_back(soldier);
    layout_->addWidget(soldier);
  }
  layout_->addWidget(ready_button_);
}

void StoreView::SetStyles() {
  setFixedSize(element_sizes::kStore);
  for (const auto& item: items_) {
    item->setStyleSheet(styles::kStoreBox);
  }
  ready_button_->setStyleSheet(styles::kPushButton);
  ready_button_->setMinimumSize(element_sizes::kDialogButton);
}

void StoreView::ConnectUI() {
  connect(ready_button_,
          &QPushButton::clicked,
          this,
          &StoreView::Ready);
}

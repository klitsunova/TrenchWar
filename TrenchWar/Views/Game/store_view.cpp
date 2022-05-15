#include "store_view.h"

#include <QLabel>

#include "helpers/sizes.h"
#include "helpers/styles.h"

StoreView::StoreView(
    QWidget* parent) : layout_(new QHBoxLayout(this)),
                       ready_button_(new QPushButton("Ready", this)),
                       build_trench_(new QPushButton("Build", this)),
                       delete_trench_(new QPushButton("Delete", this)){
  setParent(parent);
  AddItems();
  SetStyles();
  ConnectUI();
}

void StoreView::HideReadyButton() {
  ready_button_->hide();
}

void StoreView::AddItems() {
  HideTrenchButtons();
  // temporary code
  for (int i = 0; i < 5; ++i) {
    auto* soldier = new QLabel;
    soldier->setPixmap(
        PixmapLoader::GetSoldier()->scaled(element_sizes::kStoreBox));
    items_.push_back(soldier);
    layout_->addWidget(soldier);
  }
  layout_->addWidget(ready_button_);
  layout_->addWidget(build_trench_);
  layout_->addWidget(delete_trench_);
}

void StoreView::SetStyles() {
  setFixedSize(element_sizes::kStore);
  for (const auto& item : items_) {
    item->setStyleSheet(styles::kStoreBox);
  }
  ready_button_->setStyleSheet(styles::kPushButton);
  ready_button_->setMinimumSize(element_sizes::kDialogButton);
  build_trench_->setMinimumSize(element_sizes::kTrenchBuild);
  build_trench_->setStyleSheet(styles::kPushButton);
  delete_trench_->setMinimumSize(element_sizes::kTrenchBuild);
  delete_trench_->setStyleSheet(styles::kPushButton);
}

void StoreView::ConnectUI() {
  connect(ready_button_,
          &QPushButton::clicked,
          this,
          &StoreView::Ready);
  connect(build_trench_, &QPushButton::clicked, this, &StoreView::BuildTrenchButtonPressed);
  connect(delete_trench_, &QPushButton::clicked, this, &StoreView::DeleteTrenchButtonPressed);
}

void StoreView::ShowTrenchButtons() const{
  build_trench_->show();
  delete_trench_->show();
}
void StoreView::HideTrenchButtons() const {
  build_trench_ ->hide();
  delete_trench_->hide();
}

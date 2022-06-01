#include "store_view.h"

#include <QButtonGroup>
#include <QLabel>
#include <QSizePolicy>
#include <QStyleOption>
#include <QPainter>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QPixmap>

#include "helpers/sizes.h"
#include "helpers/styles.h"

StoreView::StoreView(QWidget* parent)
    : QWidget(parent),
      layout_(new QHBoxLayout(this)),
      ready_button_(new QPushButton("Ready", this)),
      build_trench_(new QPushButton("Build", this)),
      delete_trench_(new QPushButton("Delete", this)),
      modes_(new QButtonGroup(this)) {
  HideTrenchButtons();
  AddItems();
  SetStyles();
  ConnectUI();
}

void StoreView::HideReadyButton() {
  ready_button_->hide();
}

void StoreView::AddItems() {
  setLayout(layout_);
  layout_->addStretch(1);
  // temporary code
  for (int i = 0; i < 5; ++i) {
    auto* soldier = new QLabel(this);
    soldier->setPixmap(
        PixmapLoader::GetSoldier()->scaled(element_sizes::kStoreBox));
    items_.push_back(soldier);
    layout_->addWidget(soldier, 0);
  }
  modes_->addButton(new QRadioButton(this), static_cast<int>(BuyMode::kTrench));
  modes_->addButton(new QRadioButton(this), static_cast<int>(BuyMode::kUnits));

  layout_->addStretch(1);

  QVBoxLayout* mode_layout_ = new QVBoxLayout(reinterpret_cast<QWidget*>(layout_));
  mode_layout_->addWidget(modes_->button(static_cast<int>(BuyMode::kTrench)), 0);
  mode_layout_->addWidget(modes_->button(static_cast<int>(BuyMode::kUnits)), 0);
  layout_->addLayout(mode_layout_, 0);

  layout_->addWidget(ready_button_, 0);
  layout_->addWidget(build_trench_, 0);
  layout_->addWidget(delete_trench_, 0);
}

void StoreView::SetStyles() {
  for (const auto& item : items_) {
    item->setStyleSheet(styles::kStoreBox);
  }
  ready_button_->setStyleSheet(styles::kPushButton);
  ready_button_->setMinimumSize(element_sizes::kDialogButton);
  build_trench_->setMinimumSize(element_sizes::kTrenchBuild);
  build_trench_->setStyleSheet(styles::kPushButton);
  delete_trench_->setMinimumSize(element_sizes::kTrenchBuild);
  delete_trench_->setStyleSheet(styles::kPushButton);
  modes_->button(
      static_cast<int>(BuyMode::kTrench))->setStyleSheet(styles::kRadioButton);
  modes_->button(
      static_cast<int>(BuyMode::kUnits))->setStyleSheet(styles::kRadioButton);
  setStyleSheet(styles::kStoreMenu);
}

void StoreView::ConnectUI() {
  connect(ready_button_,
          &QPushButton::clicked,
          this,
          &StoreView::Ready);
  connect(build_trench_,
          &QPushButton::clicked,
          this,
          &StoreView::BuildTrenchButtonPressed);
  connect(delete_trench_,
          &QPushButton::clicked,
          this,
          &StoreView::DeleteTrenchButtonPressed);
}

void StoreView::ShowTrenchButtons() const {
  build_trench_->show();
  delete_trench_->show();
}
void StoreView::HideTrenchButtons() const {
  build_trench_ ->hide();
  delete_trench_->hide();
}

void StoreView::paintEvent(QPaintEvent*) {
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

  modes_->button(
      static_cast<int>(BuyMode::kTrench))->setWindowIcon(QPixmap(":/Resources/Images/UncheckedCommon.png").scaled(10, 10));
}

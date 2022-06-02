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
      confirm_button_(new QPushButton("Confirm", this)),
      cancel_button_(new QPushButton("Cancel", this)),
      modes_(new QButtonGroup(this)),
      money_widget_(new QWidget(this)),
      money_label_(new QLabel),
      money_layout_(new QHBoxLayout()) {
  HideTrenchButtons(); // TODO
  AddItems();
  SetLayout();
  SetStyles();
  ConnectUI();
}

void StoreView::HideReadyButton() {
  ready_button_->hide();
}

void StoreView::AddItems() {
  SetMoneyWidget();
  modes_->addButton(new QRadioButton(this), static_cast<int>(BuyMode::kTrench));
  modes_->addButton(new QRadioButton(this), static_cast<int>(BuyMode::kUnits));
  SetNames();
}

void StoreView::SetStyles() {
  for (const auto& item : items_) {
    item->setStyleSheet(styles::kStoreBox);
  }
  ready_button_->setStyleSheet(styles::kPushButton);
  ready_button_->setMinimumSize(element_sizes::kDialogButton);
  confirm_button_->setMinimumSize(element_sizes::kTrenchBuild);
  confirm_button_->setStyleSheet(styles::kPushButton);
  cancel_button_->setMinimumSize(element_sizes::kTrenchBuild);
  cancel_button_->setStyleSheet(styles::kPushButton);

  modes_->button(
      static_cast<int>(BuyMode::kTrench))->setStyleSheet(styles::kRadioButton);
  modes_->button(
      static_cast<int>(BuyMode::kUnits))->setStyleSheet(styles::kRadioButton);

  setStyleSheet(styles::kStoreMenu);

  money_widget_->setStyleSheet(styles::kStoreBox);
}

void StoreView::ConnectUI() {
  connect(ready_button_,
          &QPushButton::clicked,
          this,
          &StoreView::Ready);
  connect(confirm_button_,
          &QPushButton::clicked,
          this,
          &StoreView::ConfirmButtonPressed);
  connect(cancel_button_,
          &QPushButton::clicked,
          this,
          &StoreView::CancelButtonPressed);
}

void StoreView::ShowTrenchButtons() const {
  confirm_button_->show();
  cancel_button_->show();
}

void StoreView::HideTrenchButtons() const {
  confirm_button_ ->hide();
  cancel_button_->hide();
}

void StoreView::paintEvent(QPaintEvent*) {
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void StoreView::SetNames() {
  modes_->button(static_cast<int>(BuyMode::kTrench))->setText("Trench mode");
  modes_->button(static_cast<int>(BuyMode::kUnits))->setText("Units mode");
  // money_widget_->setPixmap(PixmapLoader::GetDollar()->scaled(element_sizes::kStoreBox));
  // money_widget_->widget()->setWindowIconText() ->setText(QString::number(count_money_));
}

void StoreView::SetLayout() {
  setLayout(layout_);
  // QLabel* label = new QLabel;
  // QString str= "<img src=\":/Resources/Images/Dollar.png\">";
  // label->setTextFormat(Qt::RichText);
  // label->setText(str);
  // layout_->addWidget(label);

  layout_->addWidget(money_widget_, 0);
  layout_->addStretch(1);

  // temporary code
  for (int i = 0; i < 5; ++i) {
    auto* soldier = new QLabel(this);
    soldier->setPixmap(
        PixmapLoader::GetSoldier()->scaled(element_sizes::kStoreBox));
    items_.push_back(soldier);
    layout_->addWidget(soldier, 0);
  }

  layout_->addStretch(1);

  QVBoxLayout* mode_layout_ = new QVBoxLayout();
  mode_layout_->addWidget(modes_->button(static_cast<int>(BuyMode::kTrench)), 0);
  mode_layout_->addWidget(modes_->button(static_cast<int>(BuyMode::kUnits)), 0);
  layout_->addLayout(mode_layout_, 0);

  layout_->addWidget(ready_button_, 0);
  layout_->addWidget(confirm_button_, 0);
  layout_->addWidget(cancel_button_, 0);
}

void StoreView::SetMoneyWidget() {
  money_widget_->setLayout(money_layout_);
  QLabel* dollar = new QLabel();
  dollar->setPixmap(PixmapLoader::GetDollar()->scaled(element_sizes::kStoreBox));
  dollar->setStyleSheet(styles::kStoreMenu);
  money_layout_->addWidget(dollar);

  money_label_->setText(QString::number(count_money_));
  money_label_->setStyleSheet(styles::kStoreMenu);
  money_layout_->addWidget(money_label_);

  // QLabel* label = new QLabel;
}

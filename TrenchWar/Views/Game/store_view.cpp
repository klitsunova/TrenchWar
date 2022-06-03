#include "store_view.h"

#include <QButtonGroup>
#include <QLabel>
#include <QStyleOption>
#include <QPainter>
#include <QRadioButton>
#include <QVBoxLayout>

#include "helpers/sizes.h"
#include "helpers/styles.h"

StoreView::StoreView(QWidget* parent)
    : QWidget(parent),
      layout_(new QHBoxLayout(this)),
      ready_button_(new QPushButton("Ready", this)),
      confirm_button_(new QPushButton("Confirm", this)),
      cancel_button_(new QPushButton("Cancel", this)),
      purchase_modes_(new QButtonGroup(this)),
      money_widget_area_(new QWidget(this)),
      money_label_(new QLabel) {
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
  purchase_modes_->addButton(new QRadioButton(this), static_cast<int>(BuyMode::kTrench));
  purchase_modes_->addButton(new QRadioButton(this), static_cast<int>(BuyMode::kUnits));
  SetMoneyWidget();
  SetNames();

  // TODO(Zolokinos) Tempo code
  price_list_["Soldier"] = 50;
}

void StoreView::SetStyles() {
  setStyleSheet(styles::kStoreMenu);

  ready_button_->setStyleSheet(styles::kPushButton);
  ready_button_->setMinimumSize(element_sizes::kDialogButton);
  confirm_button_->setMinimumSize(element_sizes::kTrenchBuild);
  confirm_button_->setStyleSheet(styles::kPushButton);
  cancel_button_->setMinimumSize(element_sizes::kTrenchBuild);
  cancel_button_->setStyleSheet(styles::kPushButton);

  purchase_modes_->button(
      static_cast<int>(BuyMode::kTrench))->setStyleSheet(styles::kRadioButton);
  purchase_modes_->button(
      static_cast<int>(BuyMode::kUnits))->setStyleSheet(styles::kRadioButton);

  money_widget_area_->setStyleSheet(styles::kStoreBoxMoneyWidget);
}

void StoreView::ConnectUI() {
  connect(ready_button_,
          &QPushButton::clicked,
          this,[&](){
    emit Ready(mode_);
  });
  connect(confirm_button_,
          &QPushButton::clicked,
          this,[&](){
    emit ConfirmButtonPressed(mode_);
  });
  connect(cancel_button_,
          &QPushButton::clicked,
          this, [&](){
    emit CancelButtonPressed(mode_);
  });
  connect(purchase_modes_,
          &QButtonGroup::buttonPressed,
          this, [&](QAbstractButton* button){
    BuyMode mode = static_cast<BuyMode>(button->group()->id(button));
    emit ModeChanged(mode);
  });
}

void StoreView::EnableStoreButtons() const {
  confirm_button_->setEnabled(true);
  cancel_button_->setEnabled(true);
}

void StoreView::HideTrenchButtons() const {
  confirm_button_ ->setDisabled(true);
  cancel_button_->setDisabled(true);
}

void StoreView::paintEvent(QPaintEvent*) {
  QStyleOption option;
  option.initFrom(this);
  QPainter painter(this);
  style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}

void StoreView::SetNames() {
  purchase_modes_->button(static_cast<int>(BuyMode::kTrench))->setText("Trench mode");
  purchase_modes_->button(static_cast<int>(BuyMode::kUnits))->setText("Units mode");
}

void StoreView::SetLayout() {
  setLayout(layout_);

  layout_->addWidget(money_widget_area_, 0);
  layout_->addStretch(1);

  QVBoxLayout* mode_layout = new QVBoxLayout();
  mode_layout->addWidget(purchase_modes_->button(static_cast<int>(BuyMode::kTrench)), 0);
  mode_layout->addWidget(purchase_modes_->button(static_cast<int>(BuyMode::kUnits)), 0);
  layout_->addLayout(mode_layout, 0);

  layout_->addWidget(confirm_button_, 0);
  layout_->addWidget(cancel_button_, 0);
  layout_->addWidget(ready_button_, 0);
}

void StoreView::SetMoneyWidget() {
  QHBoxLayout* money_link_layout = new QHBoxLayout();
  money_widget_area_->setLayout(money_link_layout);
  QLabel* dollar = new QLabel();
  dollar->setPixmap(PixmapLoader::GetDollar()->scaled(element_sizes::kStoreBox));
  dollar->setStyleSheet(styles::kStoreBoxMoneyLabels);
  money_link_layout->addWidget(dollar);

  money_label_->setText(QString::number(count_money));
  money_label_->setStyleSheet(styles::kStoreBoxMoneyLabels);
  money_link_layout->addWidget(money_label_);
}

void StoreView::FixModes() {
  purchase_modes_->button(static_cast<int>(BuyMode::kUnits))->setChecked(true);
  setDisabled(purchase_modes_);
  mode_ = BuyMode::kUnits;
}

void StoreView::SpendMoney(QString name) {
  count_money -= price_list_[name];
  money_label_->setText(QString::number(count_money));
}

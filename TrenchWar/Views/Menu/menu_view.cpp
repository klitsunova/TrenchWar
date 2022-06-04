#include "menu_view.h"

MenuView::MenuView(
    QWidget* parent) : layout_(new QVBoxLayout(this)),
                       title_label_(new QLabel(this)),
                       start_network_button_(new QPushButton("Network game",
                                                             this)),
                       start_bot_button_(new QPushButton("Bot game", this)),
                       settings_button_(new QPushButton("Settings", this)),
                       exit_button_(new QPushButton("Exit", this)),
                       background_(PixmapLoader::GetMenuBackground()),
                       title_(PixmapLoader::GetMenuTitle()) {
  setParent(parent);
  setWindowTitle("Menu");
  setWindowIcon(QIcon(":Resources/Images/Soldier1.png"));
  title_label_->setPixmap(*title_);
  SetStyles();
  SetLayout();
  ConnectUI();
}

void MenuView::SetStyles() {
  setAutoFillBackground(true);
  setBaseSize(window_sizes::kMenu);
  resize(size());
  for (auto& widget : children()) {
    auto* button_ptr = qobject_cast<QPushButton*>(widget);
    if (button_ptr != nullptr) {
      button_ptr->setFont(fonts::kMenuButton);
      button_ptr->setMinimumSize(element_sizes::kMenuButton);
      button_ptr->setStyleSheet(styles::kPushButton);
    }
  }
}

void MenuView::SetLayout() {
  layout_->setSpacing(interval_sizes::kSpacing);
  layout_->addWidget(title_label_, 1, Qt::AlignLeft);
  layout_->addStretch(interval_sizes::kStretch);
  layout_->addWidget(start_network_button_, 1, Qt::AlignRight);
  layout_->addWidget(start_bot_button_, 1, Qt::AlignRight);
  layout_->addWidget(settings_button_, 1, Qt::AlignRight);
  layout_->addWidget(exit_button_, 1, Qt::AlignRight);
}

void MenuView::ConnectUI() {
  connect(start_network_button_,
          &QPushButton::clicked,
          this,
          &MenuView::StartNetworkPressed);
  connect(start_bot_button_,
          &QPushButton::clicked,
          this,
          &MenuView::StartBotPressed);
  connect(settings_button_,
          &QPushButton::clicked,
          this,
          &MenuView::SettingsButtonPressed);
  connect(exit_button_,
          &QPushButton::clicked,
          this,
          &MenuView::ExitButtonPressed);
}

void MenuView::closeEvent(QCloseEvent* event) {
  event->ignore();
  ExitButtonPressed();
}

void MenuView::resizeEvent(QResizeEvent* event) {
  QPixmap scaled_background =
      background_->scaled(this->size(), Qt::KeepAspectRatioByExpanding);
  QPalette palette;
  palette.setBrush(QPalette::Window, scaled_background);
  setPalette(palette);

  const int title_ratio = title_label_->width() / title_label_->height();
  const double window_to_title_ratio = 1.5;
  const int new_title_width = static_cast<int>(
      size().width() / window_to_title_ratio);
  title_label_->setPixmap(title_->scaled(new_title_width,
                                         new_title_width / title_ratio));
}

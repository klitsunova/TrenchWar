#pragma once

#include <QCloseEvent>
#include <QLabel>
#include <QPalette>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "Models/Tools/pixmap_loader.h"
#include "Views/Menu/exit_window.h"
#include "helpers/fonts.h"
#include "helpers/sizes.h"
#include "helpers/styles.h"

class MenuView : public QWidget {
  Q_OBJECT

 public:
  MenuView();
  ~MenuView() override = default;

  void resizeEvent(QResizeEvent* event);

 signals:
  void StartButtonPressed();
  void SettingsButtonPressed();
  void ExitButtonPressed();

 private:
  void SetStyles();
  void SetLayout();
  void ConnectUI();

  void closeEvent(QCloseEvent* event) override;

  QVBoxLayout* layout_;
  QLabel* title_label_;
  QPushButton* start_button_;
  QPushButton* settings_button_;
  QPushButton* exit_button_;
  std::shared_ptr<QPixmap> background_;
  std::shared_ptr<QPixmap> title_;
};

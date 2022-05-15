#pragma once

#include <vector>

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

#include "Models/Tools/pixmap_loader.h"


class StoreView : public QWidget {
  Q_OBJECT
 public:
  explicit StoreView(QWidget* parent = nullptr);
  ~StoreView() = default;

  void HideReadyButton();
  void HideTrenchButtons() const;
  void ShowTrenchButtons() const;

 signals:
  void Ready();
  void BuildTrenchButtonPressed();
  void DeleteTrenchButtonPressed();

 private:
  void AddItems();
  void SetStyles();
  void ConnectUI();

  QHBoxLayout* layout_;
  QPushButton* ready_button_;
  QPushButton* build_trench_;
  QPushButton* delete_trench_;
  std::vector<QLabel*> items_;
};

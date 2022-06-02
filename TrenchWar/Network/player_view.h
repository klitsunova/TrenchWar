#pragma once

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

#include "player_model.h"

class PlayerView  : public QWidget {
  Q_OBJECT
 public:
  PlayerView(QWidget* parent, const Player* player);
  void Highlight();
  const Player* GetPlayer() const;

 private:
  const Player* player_;
  QLabel* nickname_;
  QLabel* is_ready_;
  QHBoxLayout* layout_;
};

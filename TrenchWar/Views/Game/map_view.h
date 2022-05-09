#pragma once

#include <QWidget>

#include <memory>

#include "Models/world.h"

class MapView : public QWidget {
  Q_OBJECT
 public:
  explicit MapView(QWidget* parent = nullptr,
                   const std::shared_ptr<World>& world = nullptr);
  ~MapView() = default;

  int GetScale() const;
  void SetScale(int scale);

  void paintEvent(QPaintEvent* event) override;

 private:
  int scale_;
  std::shared_ptr<World> world_;
};

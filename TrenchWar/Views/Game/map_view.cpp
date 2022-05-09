#include "map_view.h"

#include <vector>

#include <QPainter>
#include <QPixmap>
#include <QPoint>

#include "Models/GameObjects/game_object.h"
#include "helpers/sizes.h"

MapView::MapView(QWidget* parent,
                 const std::shared_ptr<World>& world) {
  setParent(parent);
  setMinimumSize(window_sizes::kWorld);
  world_ = world;
}

int MapView::GetScale() const {
  return scale_;
}

void MapView::SetScale(int scale) {
  scale_ = scale;
}

void MapView::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  const std::vector<std::shared_ptr<GameObject>>& objects = world_->GetGameObjects();
  painter.save();
  int window_width = painter.window().width() - 1;
  int window_height = painter.window().height() - 1;
  painter.drawPixmap(QRect(0, 0,
                           window_width, window_height),
                     world_->GetPixmap());
  for (const auto& object: objects) {
    int object_width = object->GetSize().width();
    int object_height = object->GetSize().height();

    QPoint screen_point;
    QPoint pos = object->GetPosition();
    screen_point.setX((window_width * (2 * pos.x() + 1))
                      / (2 * world_->GetSize().width()));
    screen_point.setY((window_height * (2 * pos.y() + 1))
                      / (2 * world_->GetSize().height()));

    QPoint top_point = QPoint(screen_point.x() - object_width / 2,
                              screen_point.y() - object_height / 2);
    QPoint bottom_point = QPoint(top_point.x() + object_width,
                                 top_point.y() + object_height);
    painter.drawPixmap(QRect(top_point, bottom_point),
                       object->GetPixmap());
  }
  painter.restore();
}

#include "map_view.h"

#include <iostream>
#include <vector>

#include <QPainter>
#include <QPixmap>
#include <QPoint>

#include "Models/GameObjects/game_object.h"
#include "helpers/sizes.h"

MapView::MapView(QWidget* parent,
                 const std::shared_ptr<World>& world) {
  setParent(parent);
  // TODO(AZYAVCHIKOV): maybe not best solution
  // setMinimumSize(window_sizes::kWorld);
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
  const std::vector<std::shared_ptr<GameObject>>& objects =
      world_->GetGameObjects();
  painter.save();
  int window_width = painter.window().width() - 1;
  int window_height = painter.window().height() - 1;

  painter.drawPixmap(QRect(0, 0,
                           window_width, window_height),
                     world_->GetPixmap());

  auto DrawObject = [&](const QPoint& pos,
                        int width, int height, const QPixmap& picture) {
    QPoint screen_point;
    screen_point.setX((window_width * (2 * pos.x() + 1))
                      / (2 * world_->GetSize().width()));
    screen_point.setY((window_height * (2 * pos.y() + 1))
                      / (2 * world_->GetSize().height()));

    QPoint top_point = QPoint(screen_point.x() - width / 2,
                              screen_point.y() - height / 2);
    QPoint bottom_point = QPoint(top_point.x() + width,
                                 top_point.y() + height);
    painter.drawPixmap(QRect(top_point, bottom_point),
                       picture);
  };

  for (const auto& object: objects) {
    DrawObject(object->GetPosition(),
               object->GetSize().width(), object->GetSize().height(),
               object->GetPixmap());
  }

  const std::vector<std::shared_ptr<Bullet>>& bullets =
      world_->GetBullets();

  for (int i = 0; i < bullets.size(); ++i) {
    DrawObject(bullets[i]->GetPosition(), 7, 7,
               bullets[i]->GetPixmap());
  }

  painter.restore();
}

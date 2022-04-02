#include "view.h"

int View::GetScale() const {
  return scale_;
}

void View::SetScale(int scale) {
  scale_ = scale;
}

void View::Update(
    QPainter* painter,
    const std::vector<std::shared_ptr<GameObject>>& objects,
    const World& world) {
  painter->save();
  int window_width = painter->window().width() - 1;
  int window_height = painter->window().height() - 1;
  painter->drawPixmap(QRect(0, 0,
                            window_width, window_height),
                      world.GetPixmap());
  int object_width = GameObject::standard_size_.width();
  int object_height = GameObject::standard_size_.height();
  for (const auto& object : objects) {
    QPoint point_on_screen;
    int x = object->GetXPosition();
    int y = object->GetYPosition();
    point_on_screen.setX((window_width * (2 * x + 1))
                         / (2 * world.GetWidth()));
    point_on_screen.setY((window_height * (2 * y + 1))
                         / (2 * world.GetHeight()));

    QPoint top_point = QPoint(point_on_screen.x() - object_width / 2,
                              point_on_screen.y() - object_height / 2);
    QPoint bottom_point = QPoint(top_point.x() + object_width,
                                 top_point.y() + object_height);
    painter->drawPixmap(QRect(top_point, bottom_point),
                        object->GetPixmap());
  }
  painter->restore();
}

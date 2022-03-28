#include "view.h"

int View::GetScale() const {
  return scale_;
}

void View::SetScale(int scale) {
  scale_ = scale;
}

void View::Update(
    QPainter* painter,
    const std::vector<std::shared_ptr<GameObject>>& objects) {
  painter->save();
  for (const auto& object : objects) {
    QPoint object_point = object->GetPosition();
    painter->setBrush(QBrush(object->GetColor(), Qt::SolidPattern));
    //  TODO: It is temporary code
    const int kObjectWidth = 10;
    const int kObjectHeight = 10;
    painter->drawRect(
        object_point.x(),
        object_point.y(),
        kObjectWidth,
        kObjectHeight);
  }
  painter->restore();
}

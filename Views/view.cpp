#include "view.h"


void View::Update(QPainter* painter, const std::vector<GameObject*>& objects) {
  painter->save();
  for (const auto& object : objects) {
    int x = object->GetPosition().first;
    int y = object->GetPosition().second;
    painter->setBrush(QBrush(object->GetColor(), Qt::SolidPattern));
    painter->drawRect(x, y, 10, 10);
  }
  painter->restore();
}

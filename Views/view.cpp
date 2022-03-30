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
  painter->restore();
}

#include "map_view.h"

#include <vector>

#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <QStyle>

#include "Models/GameObjects/game_object.h"
#include "helpers/sizes.h"
#include "helpers/styles.h"

MapView::MapView(QWidget* parent,
                 const std::shared_ptr<World>& world) {
  setParent(parent);
  // TODO(AZYAVCHIKOV): maybe not best solution
  // setMinimumSize(window_sizes::kWorld);
  buy_window_ = new BuyWindow();
  world_ = world;
}

int MapView::GetScale() const {
  return scale_;
}

void MapView::SetScale(int scale) {
  scale_ = scale;
}

void MapView::DrawObject(QPainter* painter, const QPoint& pos,
                         const QSize& size, const QPixmap& picture) {
  painter->save();
  int window_width = painter->window().width() - 1;
  int window_height = painter->window().height() - 1;
  QPoint screen_point;
  screen_point.setX((window_width * (2 * pos.x() + 1))
                        / (2 * world_->GetSize().width()));
  screen_point.setY((window_height * (2 * pos.y() + 1))
                        / (2 * world_->GetSize().height()));

  QPoint top_point = QPoint(screen_point.x() - size.width() / 2,
                            screen_point.y() - size.height() / 2);
  QPoint bottom_point = QPoint(screen_point.x() + size.width() / 2,
                               screen_point.y() + size.height() / 2);
  painter->drawPixmap(QRect(top_point, bottom_point),
                     picture);
  painter->restore();
}

void MapView::paintEvent(QPaintEvent*) {
  QPixmap buffer(this->size());
  QPainter painter;
  painter.begin(&buffer);
  painter.save();
  const std::vector<std::shared_ptr<TerrainObject>>& terrain_objects =
      world_->GetTerrainObjects();
  int window_width = painter.window().width() - 1;
  int window_height = painter.window().height() - 1;

  painter.drawPixmap(QRect(0, 0,
                           window_width + 1, window_height + 1),
                     world_->GetPixmap());
  for (const auto& object : terrain_objects) {
    DrawObject(&painter, object->GetPosition(),
               object->GetSize(), object->GetPixmap());
  }

  const std::vector<std::shared_ptr<Soldier>>& soldiers =
      world_->GetSoldiers();
  for (const auto& soldier : soldiers) {
    if (soldier->IsDead()) continue;
    DrawObject(&painter, soldier->GetPosition(),
               soldier->GetSize(), soldier->GetPixmap());
  }

  const std::vector<std::shared_ptr<Bullet>>& bullets =
      world_->GetBullets();
  for (const auto& bullet : bullets) {
    if (bullet->IsUsed()) continue;
    DrawObject(&painter, bullet->GetPosition(),
               bullet->GetSize(), bullet->GetPixmap());
  }

  painter.restore();
  painter.end();

  painter.begin(this);
  painter.drawPixmap(this->rect(), buffer);
  painter.end();
}

void MapView::mousePressEvent(QMouseEvent* event) {
  MousePressedHandler(event);
}

void MapView::mouseReleaseEvent(QMouseEvent* event) {
  MouseReleasedHandler(event);
}

void MapView::mouseDoubleClickEvent(QMouseEvent* event) {
  MouseDoubleClickedHandler(event);
}

void MapView::SetStoreDialog(QMouseEvent* event) {
  buy_window_->show();
  QPoint position = event->globalPosition().toPoint();
  buy_window_->move(position);
}

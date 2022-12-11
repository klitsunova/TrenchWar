#include "LandscapeMap.h"

#include <utility>

LandscapeMap::LandscapeMap(
    const std::vector<std::vector<Landscape>>& landscapes)
    : landscapes_(landscapes) {
  assert(!landscapes_.empty());
  size_ = QSize(landscapes_[0].size(), landscapes_.size());
  DrawMap();
}

LandscapeMap::LandscapeMap(std::vector<std::vector<Landscape>>&& landscapes)
    : landscapes_(landscapes) {
  assert(!landscapes_.empty());
  size_ = QSize(landscapes_.size(), landscapes_[0].size());
  DrawMap();
}

const QSize& LandscapeMap::GetSize() const {
  return size_;
}

bool LandscapeMap::IsTrench(const QPoint& position) const {
  // TODO(AZUAVCHIKOV) add method GetCell
  return landscapes_[position.y()][position.x()].IsTrench();
}

void LandscapeMap::MakeTrench(const QPoint& position) {
  landscapes_[position.y()][position.x()].MakeTrench();
  picture_ = std::nullopt;
}

void LandscapeMap::RemoveTrench(const QPoint& position) {
  landscapes_[position.y()][position.x()].RemoveTrench();
  picture_ = std::nullopt;
}

int LandscapeMap::GetLag(const QPoint& position) const {
  return landscapes_[position.y()][position.x()].GetTimeLag();
}

const QPixmap& LandscapeMap::GetPixmap() {
  if (!picture_.has_value()) {
    DrawMap();
  }
  assert(picture_.has_value());
  return picture_.value();
}

void LandscapeMap::DrawMap() {
  picture_.emplace(QPixmap(image_sizes::kWorldImage));
  auto& picture = picture_.value();
  auto painter = QPainter(&picture);
  painter.save();

  int window_width = painter.window().width() - 1;
  int window_height = painter.window().height() - 1;

  for (int y = 0; y < size_.height(); ++y) {
    for (int x = 0; x < size_.width(); ++x) {
      int x_top = (window_width * x) / size_.width();
      int x_bottom = ((window_width * (x + 1)) / size_.width());
      int y_top = (window_height * y) / size_.height();
      int y_bottom = ((window_height * (y + 1)) / size_.height());
      QRect cell_rect(QPoint(x_top, y_top),
                      QPoint(x_bottom, y_bottom));

      QColor color = landscapes_[y][x].GetColor();

      painter.setBrush(QBrush(color));
      painter.setPen(QPen(QColor(color), 1));

      painter.drawRect(cell_rect);
    }
  }
  painter.restore();
}


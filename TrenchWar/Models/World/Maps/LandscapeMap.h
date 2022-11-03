#pragma once

#include <QSize>
#include <QPainter>

#include "helpers/sizes.h"
#include "Models/World/Maps/Landscape/Landscape.h"
class LandscapeMap {
 public:
  explicit LandscapeMap() = default;
  explicit LandscapeMap(const std::vector<std::vector<Landscape>>& landscapes);
  explicit LandscapeMap(std::vector<std::vector<Landscape>>&& landscapes);

  const QSize& GetSize() const;

  bool IsTrench(const QPoint& position) const;
  void MakeTrench(const QPoint& position);
  void RemoveTrench(const QPoint& position);

  const QPixmap& GetPixmap();

  int GetLag(const QPoint& position) const;
  virtual int GetDistance(const QPoint& position) = 0;

 private:
  void DrawMap();

  QSize size_;
  std::vector<std::vector<Landscape>> landscapes_;
  std::optional<QPixmap> picture_{std::nullopt};
};

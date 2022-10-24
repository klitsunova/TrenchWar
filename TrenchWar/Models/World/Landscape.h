#pragma once

#include <QColor>

#include "helpers/styles.h"
class Landscape {
 public:
  enum class LandscapeType {
    TRENCH,
    STANDARD_LANDSCAPE
  };

  explicit Landscape(const QColor& color = Qt::white,
                     int move_lag = 0);

  QColor GetColor() const;

  int GetTimeLag() const;

  bool IsTrench() const;
  void MakeTrench();
  void RemoveTrench();

 protected:
  LandscapeType landscape_type_;
  QColor color_;
  int move_lag_;
};


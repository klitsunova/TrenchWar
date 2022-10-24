#include "Landscape.h"
Landscape::Landscape(const QColor& color,
                     int move_lag)
    : color_(color),
      move_lag_(move_lag),
      landscape_type_(LandscapeType::STANDARD_LANDSCAPE) {
  // TODO(AZUAVCHIKOV) remove
  assert(move_lag_ >= 0);
}

QColor Landscape::GetColor() const {
  if (IsTrench()) {
    return colors::kTrenchColor;
  }
  return color_;
}
int Landscape::GetTimeLag() const {
  return move_lag_;
}

bool Landscape::IsTrench() const {
  if (landscape_type_ == LandscapeType::TRENCH) {
    return true;
  } else {
    return false;
  }
}
void Landscape::MakeTrench() {
  landscape_type_ = LandscapeType::TRENCH;
}
void Landscape::RemoveTrench() {
  landscape_type_ = LandscapeType::STANDARD_LANDSCAPE;
}

#include "trench_update.h"
#include "helpers/styles.h"

TrenchController::TrenchController(QWidget* parent,
                                   const std::shared_ptr<World>& world,
                                   const MapView* map_view) {
  setParent(parent);
  world_ = world;
  map_view_ = map_view;
}

bool TrenchController::IsTrenchFixed() const {
  return is_trench_fixed_;
}

void TrenchController::SetTrenchFixed(bool fixed) {
  is_trench_fixed_ = fixed;
}

bool TrenchController::IsMouseClicked() const {
  return is_mouse_clicked_;
}

void TrenchController::SetMouseClicked(bool clicked) {
  is_mouse_clicked_ = clicked;
}

void TrenchController::Update() {
  QPoint start_point =
      GlobalToCellsCoordinates(start_and_end_trench_points_.first);
  QPoint end_point =
      GlobalToCellsCoordinates(start_and_end_trench_points_.second);

  if (!CheckMinimumTrenchLength(start_point, end_point)) {
    return;
  }

  QPoint shift = TakeShiftDirection(start_point, end_point);
  QPoint invert_shift =
      QPoint(1 - std::abs(shift.x()), 1 - std::abs(shift.y()));

  end_point = QPoint(end_point.x() * std::abs(shift.x())
                         + invert_shift.x() * start_point.x(),
                     end_point.y() * std::abs(shift.y())
                         + invert_shift.y() * start_point.y());
  QPoint additional_trench_1 = start_point + invert_shift;
  QPoint additional_trench_2 = start_point - invert_shift;

  while (start_point != end_point + shift) {
    DrawAndSaveTrench(additional_trench_1);
    DrawAndSaveTrench(additional_trench_2);
    DrawAndSaveTrench(start_point);
    additional_trench_1 += shift;
    additional_trench_2 += shift;
    start_point += shift;
  }
}

void TrenchController::DrawAndSaveTrench(const QPoint& pos) {
  if (!IsCorrectCell(pos)) {
    return;
  }
  changed_cells_.emplace_back(pos,
                              world_->GetCell(pos).landscape.color);
  world_->GetCell(pos).landscape.color = colors::kTrenchColor;
}

QPoint TrenchController::GlobalToCellsCoordinates(const QPoint& point) const {
  int x = world_->GetSize().width() * point.x() / map_view_->width();
  int y = world_->GetSize().height() * point.y() / map_view_->height();
  return {y, x};
}

bool TrenchController::CheckMinimumTrenchLength(const QPoint& first,
                                                const QPoint& second) {
  int delta_x = std::abs(first.x() - second.x());
  int delta_y = std::abs(first.y() - second.y());

  if (delta_x >= kMinimumTrenchLength || delta_y >= kMinimumTrenchLength) {
    return true;
  }

  return false;
}

bool TrenchController::IsCorrectCell(const QPoint& point) const {
  int max_x = world_->GetSize().width();
  int max_y = world_->GetSize().height();

  if (point.x() < 0 || point.y() < 0 ||
      point.x() > max_x - 1 || point.y() > max_y - 1) {
    return false;
  }

  return true;
}

QPoint TrenchController::TakeShiftDirection(const QPoint& start_point,
                                            const QPoint& end_point) const {
  int dx = abs(start_point.x() - end_point.x());
  int dy = abs(start_point.y() - end_point.y());

  QPoint shift;

  if (dx >= dy) {
    if (start_point.x() >= end_point.x()) {
      shift = QPoint(-1, 0);
    } else {
      shift = QPoint(1, 0);
    }
  } else {
    if (start_point.y() >= end_point.y()) {
      shift = QPoint(0, -1);
    } else {
      shift = QPoint(0, 1);
    }
  }

  return shift;
}

void TrenchController::SetSaveCellsState() {
  for (const auto& changed_cell : changed_cells_) {
    world_->GetCell(changed_cell.first).landscape.color = changed_cell.second;
  }
}

void TrenchController::SetFirstPoint(const QPoint& point) {
  start_and_end_trench_points_.first = point;
}

void TrenchController::SetSecondPoint(const QPoint& point) {
  start_and_end_trench_points_.second = point;
}

void TrenchController::ClearChangedCells() {
  changed_cells_.clear();
}

const std::vector<std::pair<QPoint, QColor>>& TrenchController::
    GetChangedCells() const {
  return changed_cells_;
}

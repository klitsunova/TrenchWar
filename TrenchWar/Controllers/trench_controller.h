#pragma once

#include <memory>
#include <QWidget>
#include <utility>
#include <vector>

#include "helpers/store.h"
#include "Models/World/world.h"
#include "Views/Game/map_view.h"

class TrenchController : public QWidget {
  Q_OBJECT

 public:
  explicit TrenchController(QWidget* parent = nullptr,
                            const std::shared_ptr<World>& world = nullptr,
                            const MapView* map_view = nullptr);

  bool IsTrenchFixed() const;
  void SetTrenchFixed(bool fixed);

  bool IsMouseClicked() const;
  void SetMouseClicked(bool clicked);

  void SetFirstPoint(const QPoint& point);
  void SetSecondPoint(const QPoint& point);

  void Update();
  void SetSaveCellsState();

  void ClearChangedCells();

  int GetTrenchLength();

  const std::vector<QPoint>& GetChangedCells() const;

 private:
  void DrawAndSaveTrench(const QPoint& pos);
  QPoint GlobalToCellsCoordinates(const QPoint& point) const;
  bool CheckMinimumTrenchLength(const QPoint& first, const QPoint& second);
  bool IsCorrectCell(const QPoint& point) const;
  QPoint TakeShiftDirection(const QPoint& start_point,
                            const QPoint& end_point) const;

  std::vector<QPoint> changed_cells_;
  std::pair<QPoint, QPoint> start_and_end_trench_points_ =
      std::make_pair(QPoint(0, 0), QPoint(0, 0));
  bool is_trench_fixed_{false};
  bool is_mouse_clicked_{false};

  std::shared_ptr<World> world_;
  const MapView* map_view_;
};

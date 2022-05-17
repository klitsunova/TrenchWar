#pragma once

#include <memory>
#include <vector>
#include <utility>

#include <QBasicTimer>
#include <QKeyEvent>
#include <QShortcut>
#include <QWidget>

#include "Controllers/game_controller.h"
#include "Views/Game/game_view.h"

class EventsController : public QWidget {
  Q_OBJECT

 public:
  enum class Stage {
    kPreparation,
    kActive,
  };

  explicit EventsController(QWidget* parent = nullptr);
  ~EventsController() override = default;

  void Start();
  void HideGame();
  void SetFullScreen(bool is_fullscreen);

  void BuildTrench();
  void DeleteTrench();

  void StartTimer();
  void PauseTimer();
  void timerEvent(QTimerEvent*) override;

  Stage GetGameStage() const;

  void MapPressEvent(QMouseEvent* event);
  void MapMoveEvent(QMouseEvent* event);
  void MapReleaseEvent(QMouseEvent* event);

 signals:
  void ShowPauseMenu();

 private:
  void TrenchUpdate();
  void DrawAndSaveTrench(const QPoint& pos);
  QPoint GlobalToCellsCoordinates(const QPoint& point) const;
  bool CheckMinimumTrenchLength(const QPoint& first, const QPoint& second);
  bool IsCorrectCell(const QPoint& point) const;
  QPoint TakeShiftDirection(const QPoint& first, const QPoint& second) const;
  void SetSaveCellsState();

  QColor trench_color_ = QColor(92, 64, 51);
  std::pair<QPoint, QPoint> start_and_end_trench_points_ =
      std::make_pair(QPoint(0, 0), QPoint(0, 0));
  bool is_trench_fixed_;
  bool is_mouse_clicked_;

  std::vector<std::pair<QPoint, QColor>> changed_cells_;

  static constexpr int kMinimumTrenchLength{5};
  static constexpr int kTimerInterval{10};

  void ConnectUI();

  std::shared_ptr<World> world_;
  std::unique_ptr<GameView> view_;
  std::unique_ptr<QBasicTimer> timer_;
  std::unique_ptr<GameController> game_controller_;

  Stage game_stage = Stage::kPreparation;
};

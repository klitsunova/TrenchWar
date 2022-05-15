#include "events_controller.h"
#include <iostream>

EventsController::EventsController(QWidget* parent) {
  setParent(parent);
  world_ = std::make_shared<World>(":Resources/Maps/map1.txt");
  view_ = std::make_unique<GameView>(
      this, world_,
      [&](QMouseEvent* event) { MapPressEvent(event); return; },
      [&](QMouseEvent* event) { MapMoveEvent(event); return; },
      [&](QMouseEvent* event) { MapReleaseEvent(event); return; });
  timer_ = std::make_unique<QBasicTimer>();
  game_controller_ = std::make_unique<GameController>(this, world_);
  game_controller_->SetWorldObjects();
  ConnectUI();
  view_->show();
}

void EventsController::timerEvent(QTimerEvent*) {
  for (const auto& soldier: world_->GetSoldiers()) {
    soldier->MoveSoldier(world_->GetSize());
  }
  if (game_stage == Stage::kPreparation) {
    TrenchUpdate();
  }
  view_->UpdateMap();
}

void EventsController::StartTimer() {
  if (!timer_->isActive()) {
    timer_->start(kTimerInterval, this);
  }
}

void EventsController::PauseTimer() {
  if (timer_->isActive()) {
    timer_->stop();
  }
}

void EventsController::ConnectUI() {
  connect(view_.get(),
          &GameView::Close,
          this,
          &EventsController::ShowPauseMenu);
  connect(view_.get(),
          &GameView::StartGame,
          this,
          &EventsController::Start);
  connect(view_->GetStore(),
          &StoreView::BuildTrenchButtonPressed,
          this,
          &EventsController::BuildTrench);
  connect(view_->GetStore(),
          &StoreView::DeleteTrenchButtonPressed,
          this,
          &EventsController::DeleteTrench);
}

void EventsController::HideGame() {
  view_->hide();
}

void EventsController::Start() {
  view_->HideReadyButton();
  game_stage = Stage::kActive;
  StartTimer();
}

EventsController::Stage EventsController::GetGameStage() const {
  return game_stage;
}

void EventsController::SetFullScreen(bool is_fullscreen) {
  view_->SetFullScreen(is_fullscreen);
}

void EventsController::MapPressEvent(QMouseEvent* event) {
  if (!is_trench_fixed_) {
    is_mouse_clicked_ = true;
    start_and_end_trench_points_.first = event->pos();
    start_and_end_trench_points_.second = event->pos();
  }
}

void EventsController::MapMoveEvent(QMouseEvent* event) {
    if (is_mouse_clicked_ && !is_trench_fixed_) {
      SetSaveCellsState();
      changed_cells_.clear();
      start_and_end_trench_points_.second = event->pos();
      TrenchUpdate();
    }
}

void EventsController::MapReleaseEvent(QMouseEvent* event) {
  is_mouse_clicked_ = false;
  start_and_end_trench_points_.second = event->pos();

  QPoint start_point = GlobalToCellsCoordinates(start_and_end_trench_points_.first);
  QPoint end_point = GlobalToCellsCoordinates(start_and_end_trench_points_.second);

  if (!CheckMinimumTrenchLength(start_point, end_point)
      || (!IsCorrectCell(start_point) && !IsCorrectCell(end_point))) {
    is_trench_fixed_ = false;
  } else {
    is_trench_fixed_ = true;
  }

  if (!is_trench_fixed_) {
    SetSaveCellsState();
    view_->UpdateMap();
    changed_cells_.clear();
    return;
  }

  view_->GetStore()->ShowTrenchButtons();
}

void EventsController::TrenchUpdate() {
  //std::cout << "Draw";
  QPoint start_point = GlobalToCellsCoordinates(start_and_end_trench_points_.first);
  QPoint end_point = GlobalToCellsCoordinates(start_and_end_trench_points_.second);

  if (!CheckMinimumTrenchLength(start_point, end_point)) {
    return;
  }

  QPoint shift = TakeShiftDirection(start_point, end_point);
  QPoint invert_shift = QPoint(1 - std::abs(shift.x()), 1 - std::abs(shift.y()));

  end_point = QPoint(end_point.x() * std::abs(shift.x()) + invert_shift.x() * start_point.x(),
                     end_point.y() * std::abs(shift.y()) + invert_shift.y() * start_point.y());
  QPoint additional_trench_1 = start_point + invert_shift;
  QPoint additional_trench_2 = start_point - invert_shift;
  while (start_point != end_point + shift) {
    DrawAndSaveTrench(additional_trench_1);
    DrawAndSaveTrench(additional_trench_2);
    DrawAndSaveTrench(start_point);
    additional_trench_1 += shift;
    additional_trench_2 += shift;
    start_point += shift;
    view_->UpdateMap();
  }
}

void EventsController::DrawAndSaveTrench(const QPoint& pos) {
  if (!IsCorrectCell(pos)) {
    return;
  }
    changed_cells_.emplace_back(pos,
                                world_->GetCell(pos).landscape.color);
    world_->GetCell(pos).landscape.color = trench_color_;
}

QPoint EventsController::GlobalToCellsCoordinates(const QPoint& point) const {
  int x = world_->GetSize().width() * point.x() / view_->GetMap()->width();
  int y = world_->GetSize().height() * point.y() / view_->GetMap()->height();
  return {y, x};
}

bool EventsController::CheckMinimumTrenchLength(const QPoint& first, const QPoint& second) {
  int delta_x = std::abs(first.x() - second.x());
  int delta_y = std::abs(first.y() - second.y());

  if (delta_x >= kMinimumTrenchLength || delta_y >= kMinimumTrenchLength) {
    return true;
  }

  return false;
}

bool EventsController::IsCorrectCell(const QPoint& point) const {
  int max_x = world_->GetSize().width();
  int max_y = world_->GetSize().height();

  if (point.x() < 0 || point.y() < 0 || point.x() > max_x - 1 || point.y() > max_y - 1) {
    return false;
  }

  return true;
}

QPoint EventsController::TakeShiftDirection(const QPoint& first, const QPoint& second) const {
  int delta_x = abs(first.x() - second.x());
  int delta_y = abs(first.y() - second.y());

  QPoint shift;

  if (delta_x >= delta_y) {
    if (first.x() >= second.x()) {
      shift = QPoint(-1, 0);
    } else {
      shift = QPoint(1, 0);
    }
  } else {
    if (first.y() >= second.y()) {
      shift = QPoint(0, -1);
    } else {
      shift = QPoint(0, 1);
    }
  }

  return shift;
}

void EventsController::SetSaveCellsState() {
  for (auto& changed_cell: changed_cells_) {
    world_->GetCell(changed_cell.first).landscape.color = changed_cell.second;
  }
}

void EventsController::BuildTrench() {
  for (auto& changed_cell: changed_cells_) {
    world_->GetCell(changed_cell.first).is_trench = true;
  }
  changed_cells_.clear();
  view_->GetStore()->HideTrenchButtons();
  is_trench_fixed_ = false;
}

void EventsController::DeleteTrench() {
  SetSaveCellsState();
  view_->UpdateMap();
  changed_cells_.clear();
  view_->GetStore()->HideTrenchButtons();
  is_trench_fixed_ = false;
}

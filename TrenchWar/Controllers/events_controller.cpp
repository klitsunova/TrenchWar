#include "events_controller.h"

EventsController::EventsController(QWidget* parent) {
  setParent(parent);
  world_ = std::make_shared<World>(":Resources/Maps/map2.txt");
  view_ = std::make_unique<GameView>(this, world_);
  trench_controller_ = std::make_unique<TrenchController>(this,
                                                          world_,
                                                          view_->GetMap());
  timer_ = std::make_unique<QBasicTimer>();
  game_controller_ = std::make_unique<GameController>(this, world_);
  game_controller_->SetWorldObjects();
  ConnectUI();
  view_->show();
}

void EventsController::timerEvent(QTimerEvent*) {
  world_->FireTower();
  world_->MoveSoldiers();
  world_->MakeShots();
  world_->MoveBullets();
  world_->Update();
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
  connect(view_->GetMap(),
          &MapView::MouseReleasedHandler,
          this,
          &EventsController::MapReleaseHandler);
  connect(view_->GetMap(),
          &MapView::MousePressedHandler,
          this,
          &EventsController::MapPressHandler);
}

void EventsController::HideGame() {
  view_->hide();
}

void EventsController::Start() {
  DeleteTrench();
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

void EventsController::MapPressHandler(QMouseEvent* event) {
  if (!trench_controller_->IsTrenchFixed()
      && game_stage == Stage::kPreparation) {
    trench_controller_->SetMouseClicked(true);
    trench_controller_->SetFirstPoint(event->pos());
    trench_controller_->SetSecondPoint(event->pos());
  }
}

void EventsController::MapReleaseHandler(QMouseEvent* event) {
  if (trench_controller_->IsTrenchFixed()
      || game_stage != Stage::kPreparation) {
    return;
  }

  trench_controller_->SetSecondPoint(event->pos());
  trench_controller_->Update();
  world_->TrenchUpdate();
  view_->UpdateMap();

  trench_controller_->SetMouseClicked(true);
  trench_controller_->SetTrenchFixed(
      !trench_controller_->GetChangedCells().empty());

  if (!trench_controller_->IsTrenchFixed()) {
    trench_controller_->SetSaveCellsState();
    world_->TrenchUpdate();
    view_->UpdateMap();
    trench_controller_->ClearChangedCells();
    return;
  }

  view_->GetStore()->ShowTrenchButtons();
}

void EventsController::BuildTrench() {
  for (const auto& changed_cell : trench_controller_->GetChangedCells()) {
    world_->GetCell(changed_cell.first).is_trench = true;
  }
  trench_controller_->ClearChangedCells();
  view_->GetStore()->HideTrenchButtons();
  trench_controller_->SetTrenchFixed(false);
}

void EventsController::DeleteTrench() {
  trench_controller_->SetSaveCellsState();
  world_->TrenchUpdate();
  view_->UpdateMap();
  trench_controller_->ClearChangedCells();
  view_->GetStore()->HideTrenchButtons();
  trench_controller_->SetTrenchFixed(false);
}

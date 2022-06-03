#include "events_controller.h"

#include "Network/network_view.h"
#include "Models/Tools/settings.h"

#include <QMessageBox>

EventsController::EventsController(QWidget* parent) {
  setParent(parent);
  network_view_ = std::make_unique<NetworkView>(this);
  network_view_->show();
  connect(network_view_.get(),
          &NetworkView::StartGame,
          this,
          &EventsController::StartPreparationStage);
  connect(network_view_.get(),
          &NetworkView::ReturnToMainMenu,
          this,
          &EventsController::ReturnToMainMenu);
}

void EventsController::timerEvent(QTimerEvent*) {
  world_->MoveSoldiers();
  world_->MakeShots();
  world_->MoveBullets();
  world_->FireTower();
  view_->UpdateMap();
  world_->Update();
  CheckGameEnding();
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
          &EventsController::SetPreparedStatus);
  connect(network_controller_.get(),
          &NetworkController::GotSignalForActiveStage,
          this,
          &EventsController::StartActiveStage);
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
  connect(view_.get(),
          &GameView::GameFinishedEvent,
          this,
          &EventsController::ReturnToMainMenu);
}

void EventsController::HideGame() {
  if (network_view_) {
    network_view_->hide();
  }
  if (view_) {
    view_->hide();
  }
}

void EventsController::StartPreparationStage() {
  emit HideMainMenu();
  network_view_->hide();
  network_controller_ = network_view_->GetNetworkController();
  world_ = std::make_shared<World>(":Resources/Maps/map2.txt");
  view_ = std::make_unique<GameView>(this, world_);
  trench_controller_ = std::make_unique<TrenchController>(this,
                                                          world_,
                                                          view_->GetMap());
  timer_ = std::make_unique<QBasicTimer>();
  game_controller_ = std::make_unique<GameController>(this, world_);
  game_controller_->SetWorldObjects(network_view_->GetPlayerSide());
  ConnectUI();
  view_->SetFullScreen(Settings::Instance()->IsFullScreen());
  view_->show();
}

void EventsController::SetPreparedStatus() {
  if (network_view_->GetPlayerSide() == Side::kAttacker) {
    network_controller_->SetAttackersData(game_controller_->GetAttackersData());
  } else {
    network_controller_->SetDefendersData(game_controller_->GetDefendersData());
  }
  network_controller_->SendData();
}

void EventsController::StartActiveStage() {
  if (network_view_->GetPlayerSide() == Side::kAttacker) {
    game_controller_->UpdateDefenders(network_controller_->GetDefendersData());
  } else {
    game_controller_->UpdateAttackers(network_controller_->GetAttackersData());
  }
  DeleteTrench();
  view_->HideReadyButton();
  game_stage = Stage::kActive;
  world_->UpdateCountAttackers();
  StartTimer();
}

EventsController::Stage EventsController::GetGameStage() const {
  return game_stage;
}

void EventsController::SetFullScreen(bool is_fullscreen) {
  if (view_) {
    view_->SetFullScreen(is_fullscreen);
  }
}

void EventsController::MapPressHandler(QMouseEvent* event) {
  if (network_view_->GetPlayerSide() == Side::kDefender
      && !trench_controller_->IsTrenchFixed()
      && game_stage == Stage::kPreparation) {
    trench_controller_->SetMouseClicked(true);
    trench_controller_->SetFirstPoint(event->pos());
    trench_controller_->SetSecondPoint(event->pos());
  }
}

void EventsController::MapReleaseHandler(QMouseEvent* event) {
  if (network_view_->GetPlayerSide() == Side::kAttacker
      || trench_controller_->IsTrenchFixed()
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

void EventsController::CheckGameEnding() {
  int attackers = world_->GetCountAttackers();
  int towers = world_->GetCountTowers();
  Side player = network_controller_->GetPlayerSide();

  if (attackers == 0 || towers == 0) {
    PauseTimer();
  }

  if (attackers == 0 && towers == 0) {
    view_->SetDrawState();
  }

  if ((attackers == 0 && player == Side::kDefender) ||
      (towers == 0 && player == Side::kAttacker)) {
    view_->SetWinState();
  }

  if ((attackers == 0 && player == Side::kAttacker) ||
      (towers == 0 && player == Side::kDefender)) {
    view_->SetLoseState();
  }
}

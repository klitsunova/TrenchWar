#include <iostream>
#include "events_controller.h"

#include <random>

#include <QMessageBox>

#include "Models/Tools/settings.h"
#include "Network/network_view.h"

EventsController::EventsController(QWidget* parent, GameMode mode)
    : game_mode_(mode),
      game_finish_window_(new GameFinishWindow()),
      player_(new QMediaPlayer(this)) {

  setParent(parent);
  std::random_device rd;
  std::uniform_int_distribution<int> distribution(0, 1);
  player_side_ = static_cast<Side>(distribution(rd));
  if (game_mode_ == GameMode::kNetwork) {
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
  } else {
    StartPreparationStage();
  }
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
  connect(view_.get(),
          &GameView::ConfirmButtonPressed,
          this,
          &EventsController::ConfirmPurchase);
  connect(view_.get(),
          &GameView::CancelButtonPressed,
          this,
          &EventsController::CancelPurchase);
  connect(view_->GetMap(),
          &MapView::MouseReleasedHandler,
          this,
          &EventsController::MapReleaseHandler);
  connect(view_->GetMap(),
          &MapView::MousePressedHandler,
          this,
          &EventsController::MapPressHandler);
  connect(view_->GetMap(),
          &MapView::MouseDoubleClickedHandler,
          this,
          &EventsController::MapDoubleClickHandler);
  connect(view_->GetStore(),
          &StoreView::ModeChanged,
          this,
          &EventsController::ChangeMode);
  connect(world_.get(),
          &World::Shot,
          this,
          &EventsController::Shot);
  connect(game_finish_window_,
          &GameFinishWindow::ToMenu,
          this,
          &EventsController::CloseFinishWindow);
}

void EventsController::HideGame() {
  if (network_view_) {
    network_view_->Disconnect();
    network_view_->hide();
  }
  if (view_) {
    view_->hide();
  }
}

void EventsController::StartPreparationStage() {
  emit HideMainMenu();
  world_ = std::make_shared<World>(":Resources/Maps/map2.txt",
                                   game_mode_,
                                   player_side_);
  view_ = std::make_unique<GameView>(this, world_);
  trench_controller_ =
      std::make_unique<TrenchController>(this, world_, view_->GetMap());
  timer_ = std::make_unique<QBasicTimer>();
  game_controller_ = std::make_unique<GameController>(this, world_);

  if (game_mode_ == GameMode::kNetwork) {
    network_view_->hide();
    network_controller_ = network_view_->GetNetworkController();
    player_side_ = network_view_->GetPlayerSide();
    connect(network_controller_.get(),
            &NetworkController::GotSignalForActiveStage,
            this,
            &EventsController::StartActiveStage);
  }

  ConnectUI();
  view_->SetFullScreen(Settings::IsFullScreen());
  view_->show();
}

void EventsController::SetPreparedStatus() {
  if (game_mode_ == GameMode::kBot) {
    StartActiveStage();
    return;
  }
  if (player_side_ == Side::kAttacker) {
    network_controller_->SetAttackersData(game_controller_->GetAttackersData());
  } else {
    network_controller_->SetDefendersData(game_controller_->GetDefendersData());
  }
  network_controller_->SendData();
}

void EventsController::StartActiveStage() {
  CancelPurchase(buy_mode_);
  view_->Start();
  view_->GetStore()->setVisible(false);
  if (game_mode_ == GameMode::kNetwork) {
    if (player_side_ == Side::kAttacker) {
      game_controller_->UpdateDefenders(
          network_controller_->GetDefendersData());
    } else {
      game_controller_->UpdateAttackers(
          network_controller_->GetAttackersData());
    }
  } else {
    Side bot_side = (player_side_ == Side::kAttacker)
                    ? Side::kDefender
                    : Side::kAttacker;
    world_->LoadBotData(bot_side);
  }
  CancelPurchase(buy_mode_);
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
  switch (buy_mode_) {
    case BuyMode::kUnits: {
      break;
    }
    case BuyMode::kTrench: {
      if (player_side_ == Side::kDefender
          && !trench_controller_->IsTrenchFixed()
          && game_stage == Stage::kPreparation) {
        trench_controller_->SetMouseClicked(true);
        trench_controller_->SetFirstPoint(event->pos());
        trench_controller_->SetSecondPoint(event->pos());
      }
    }
  }
}

void EventsController::MapReleaseHandler(QMouseEvent* event) {
  switch (buy_mode_) {
    case BuyMode::kUnits: {
      break;
    }
    case BuyMode::kTrench: {
      if (player_side_ == Side::kAttacker
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

      int cost = trench_controller_->GetTrenchLength();
      view_->GetStore()->ShowCost(cost);

      if (!trench_controller_->IsTrenchFixed()) {
        trench_controller_->SetSaveCellsState();
        world_->TrenchUpdate();
        view_->UpdateMap();
        trench_controller_->ClearChangedCells();
        return;
      }
      view_->GetStore()->EnableStoreButtons();
      break;
    }
  }
}

void EventsController::MapDoubleClickHandler(QMouseEvent* event) {
  switch (buy_mode_) {
    case BuyMode::kUnits: {
      view_->SetStoreDialog(event);
      break;
    }
    case BuyMode::kTrench: {
      break;
    }
  }
}

void EventsController::ConfirmPurchase(BuyMode mode, QString name) {
  switch (mode) {
    case BuyMode::kTrench: {
      if ((view_->GetStore()->SpendMoney(name))) {
        for (const auto& changed_cell : trench_controller_->GetChangedCells()) {
          world_->GetCell(changed_cell.first).is_trench = true;
        }
        trench_controller_->ClearChangedCells();
        view_->GetStore()->HideTrenchButtons();
        trench_controller_->SetTrenchFixed(false);
        view_->GetStore()->ClearToSpendMoneyLabel();
      }
      break;
    }
    case BuyMode::kUnits: {
      QPoint location;
      if (view_->GetStore()->SpendMoney(name)) {
        std::cerr << "Unit\n";
        location = view_->GetMap()->GetBuyWindow()->GetLocation();
        int window_width = view_->GetMap()->geometry().width() - 1;
        int window_height = view_->GetMap()->geometry().height() - 1;

        QPoint game_point;
        game_point.setX(world_->GetSize().width() * (location.x()
            - view_->GetMap()->mapToGlobal(QPoint(0, 0)).x())
                            / window_width);
        game_point.setY(world_->GetSize().height() * (location.y()
            - view_->GetMap()->mapToGlobal(QPoint(0, 0)).y())
                            / window_height);

        world_->AddSoldier(game_point, Side::kAttacker);
        world_->Update();
        view_->UpdateMap();
      }
      break;
    }
  }
}

void EventsController::CancelPurchase(BuyMode mode, QString name) {
  switch (mode) {
    case BuyMode::kTrench: {
      trench_controller_->SetSaveCellsState();
      world_->TrenchUpdate();
      view_->UpdateMap();
      trench_controller_->ClearChangedCells();
      view_->GetStore()->HideTrenchButtons();
      view_->GetStore()->ClearToSpendMoneyLabel();
      trench_controller_->SetTrenchFixed(false);
      break;
    }
    case BuyMode::kUnits: {
      view_->GetMap()->GetBuyWindow()->ClearWindow();
      view_->GetMap()->GetBuyWindow()->Close();
      break;
    }
  }
}

void EventsController::ChangeMode(BuyMode mode) {
  buy_mode_ = mode;
  switch (mode) {
    case BuyMode::kUnits: {
      CancelPurchase(BuyMode::kTrench);
      break;
    }
    case BuyMode::kTrench: {
      CancelPurchase(BuyMode::kUnits);
      break;
    }
  }
}

void EventsController::Shot() {
  player_->setLoops(1);
  player_->play();
}

void EventsController::CheckGameEnding() {
  int attackers = world_->GetCountAttackers();
  int towers = world_->GetCountTowers();

  if (attackers == 0 || towers == 0) {
    PauseTimer();
  }

  if (attackers == 0 && towers == 0) {
    game_finish_window_->Show(GameFinishWindow::States::kDraw);
  }

  if ((attackers == 0 && player_side_ == Side::kDefender) ||
      (towers == 0 && player_side_ == Side::kAttacker)) {
    game_finish_window_->Show(GameFinishWindow::States::kWin);
  }

  if ((attackers == 0 && player_side_ == Side::kAttacker) ||
      (towers == 0 && player_side_ == Side::kDefender)) {
    game_finish_window_->Show(GameFinishWindow::States::kLose);
  }
}

void EventsController::CloseFinishWindow() {
  game_finish_window_->hide();
  ReturnToMainMenu();
}

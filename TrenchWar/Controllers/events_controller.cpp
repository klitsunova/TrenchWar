#include <iostream>
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
}

void EventsController::HideGame() {
  view_->hide();
}

void EventsController::Start(BuyMode mode) {
  CancelPurchase(mode);
  view_->Start();
  view_->GetStore()->setVisible(false);
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
  switch (mode_) {
    case BuyMode::kUnits: {
      break;
    }
    case BuyMode::kTrench: {
      if (!trench_controller_->IsTrenchFixed()
          && game_stage == Stage::kPreparation) {
        trench_controller_->SetMouseClicked(true);
        trench_controller_->SetFirstPoint(event->pos());
        trench_controller_->SetSecondPoint(event->pos());
      }
    }
  }
}

void EventsController::MapReleaseHandler(QMouseEvent* event) {
  std::cout << static_cast<int>(mode_) << std::endl;
  switch (mode_) {
    case BuyMode::kUnits: {
      break;
    }
    case BuyMode::kTrench: {
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
  switch (mode_) {
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
  mode_ = mode;
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

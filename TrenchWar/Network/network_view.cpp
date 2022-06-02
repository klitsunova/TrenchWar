#include "network_view.h"

#include "helpers/fonts.h"
#include "helpers/sizes.h"
#include "helpers/styles.h"

NetworkView::NetworkView(QWidget* parent)
    : back_to_main_menu_(new QPushButton("Back", this)),
      try_connect_(new QPushButton("Connect", this)),
      ready_(new QPushButton("Ready", this)),
      disconnect_(new QPushButton("Disconnect", this)),
      ip_(new QLineEdit("127.0.0.1", this)),
      connection_status_(new QLabel("Not connected", this)),
      main_layout_(new QVBoxLayout(this)),
      connection_layout_(new QVBoxLayout()),
      buttons_layout_(new QHBoxLayout()),
      players_layout_(new QVBoxLayout()),
      network_player_(new Player(new QTcpSocket())) {
  SetStyles();
  SetUpLayouts();
  ConnectButtons();
  show();
}

void NetworkView::ConnectButtons() const {
  connect(back_to_main_menu_,
          &QPushButton::clicked,
          this,
          &NetworkView::ReturnToMainMenu);
  connect(try_connect_,
          &QPushButton::clicked,
          this,
          &NetworkView::Connect);
  connect(ready_,
          &QPushButton::clicked,
          this,
          &NetworkView::ChangeReadyStatus);
  connect(disconnect_,
          &QPushButton::clicked,
          this,
          &NetworkView::Disconnect);
}

void NetworkView::SetUpLayouts() {
  main_layout_->addLayout(buttons_layout_);
  main_layout_->addLayout(connection_layout_);
  main_layout_->addLayout(players_layout_);
  connection_status_->setAlignment(Qt::AlignCenter);
  connection_layout_->addWidget(ip_);
  connection_layout_->addWidget(connection_status_);
  connection_layout_->addWidget(disconnect_);
  connection_layout_->addWidget(try_connect_);
  buttons_layout_->addWidget(back_to_main_menu_);
  buttons_layout_->addWidget(ready_);
}

void NetworkView::SetStyles() {
  setStyleSheet(styles::kWidget);
  for (auto& widget : children()) {
    auto* label_ptr = qobject_cast<QLabel*>(widget);
    auto* button_ptr = qobject_cast<QPushButton*>(widget);
    if (label_ptr) {
      label_ptr->setFont(fonts::kDialogLabel);
    } else if (button_ptr) {
      button_ptr->setFont(fonts::kDialogButton);
      button_ptr->setMinimumSize(element_sizes::kDialogButton);
      button_ptr->setStyleSheet(styles::kPushButton);
    }
  }
  ip_->setStyleSheet(styles::kLineEdit);
}

void NetworkView::Connect() {
  if (network_player_->Socket()->state() == QAbstractSocket::ConnectedState) {
    connection_status_->setText("Already connected");
    return;
  }
  network_player_->Socket()->connectToHost(ip_->text(), Network::kPort);
  network_player_->Socket()->waitForConnected(kMillisWaitForConnection);
  if (network_player_->Socket()->state() != QAbstractSocket::ConnectedState) {
    connection_status_->setText("Connection Error");
    return;
  }
  connection_status_->setText("Connected Successfully");
  network_controller_ = std::make_shared<NetworkController>(network_player_);
  connect(network_controller_.get(),
          &NetworkController::StartGame,
          this,
          &NetworkView::StartGame);
  connect(network_controller_.get(),
          &NetworkController::GotPlayersVector,
          this,
          &NetworkView::UpdatePlayersVector);
  connect(network_controller_.get(),
          &NetworkController::GotSignalToStart,
          this,
          &NetworkView::StartGame);
}

void NetworkView::ChangeReadyStatus() {
  if (network_player_->Socket()->state() == QAbstractSocket::ConnectedState) {
    network_player_->SetReady(!network_player_->IsReady());
    network_controller_->SendReadyStatus();
  } else {
    connection_status_->setText("You are not connected!");
  }
}

void NetworkView::SetUpAndStartGame() {
  if (!network_controller_->IsStarted()) {
    emit StartGame();
  }
}

void NetworkView::UpdatePlayersVector() {
  QString json = network_controller_->GetData().toString();
  auto data_vector = JsonHelper::DecodePlayersData(json);

  QLayoutItem* item;
  while ((item = players_layout_->takeAt(0)) != nullptr) {
    delete item->widget();
    delete item;
  }
  if (data_vector.size() < players_.size()) {
    Disconnect();
    return;
  }
  players_.clear();
  for (const auto& data : data_vector) {
    auto* player = new Player(nullptr);
    player->SetId(data.id);
    player->SetSide(static_cast<Side>(data.side));
    player->SetReady(data.status);
    if (data.id == network_player_->GetId()) {
      network_player_->SetSide(static_cast<Side>(data.side));
    }
    players_.emplace_back(new PlayerView(this, player));
    players_layout_->addWidget(players_.back());
  }
  if (!is_first_packet_received_) {
    network_player_->SetId(players_.size() - 1);
    is_first_packet_received_ = true;
    if (network_player_->GetId() == 0) {
      AddStartButton();
    }
  }
  players_[network_player_->GetId()]->Highlight();
}

void NetworkView::AddStartButton() {
  start_button_ = new QPushButton("Start", this);
  start_button_->setFont(fonts::kDialogButton);
  start_button_->setMinimumSize(element_sizes::kDialogButton);
  start_button_->setStyleSheet(styles::kPushButton);
  buttons_layout_->addWidget(start_button_);
  connect(start_button_,
          &QPushButton::clicked,
          this,
          &NetworkView::PrepareForStart);
}

void NetworkView::PrepareForStart() {
  for (const auto& player : players_) {
    if (!player->GetPlayer()->IsReady()) {
      connection_status_->setText("Error! Not everybody is ready");
      return;
    }
  }
  network_controller_->SetStarted(true);
  network_controller_->SendStartSignal();
  SetUpAndStartGame();
}

void NetworkView::SetEndPreparationStatus() {
  network_controller_->SendEndPreparationStatus();
}

void NetworkView::Disconnect() {
  if (network_player_->Socket()->state() != QAbstractSocket::ConnectedState) {
    connection_status_->setText("You are not connected to disconnect!");
    return;
  }
  if (network_controller_->IsStarted()) {
    emit ExitDisconnected();
  }
  network_player_->Socket()->disconnectFromHost();
  QLayoutItem* item;
  while ((item = players_layout_->takeAt(0)) != nullptr) {
    delete item->widget();
    delete item;
  }
  connection_status_->setText("Disconnected from the server");
  buttons_layout_->removeWidget(start_button_);
  delete start_button_;
  start_button_ = nullptr;
  players_.clear();
  network_player_->SetReady(false);
  network_controller_.reset();
  network_controller_ = nullptr;
  is_first_packet_received_ = false;
}

std::shared_ptr<NetworkController> NetworkView::GetNetworkController() const {
  return network_controller_;
}

Side NetworkView::GetPlayerSide() const {
  return network_player_->GetSide();
}

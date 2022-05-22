#include "server.h"

#include <string>

#include <QNetworkInterface>
#include <QTcpSocket>

Server::Server() : ip_(this),
                   server_(this) {
  server_.listen(QHostAddress::Any, Network::kPort);
  resize(300, 100);
  connect(&server_,
          &QTcpServer::newConnection,
          this,
          &Server::ConnectClient);
  ShowIp();
}

void Server::ShowIp() {
  QString ip_addresses;
  QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
  for (const auto& address: addresses) {
    ip_addresses += address.toString();
    ip_addresses += '\n';
  }
  ip_.setText(ip_addresses);
}

void Server::ConnectClient() {
  players_.emplace_back(std::make_shared<Player>(server_.nextPendingConnection()));
  players_.back()->SetId(players_.size() - 1);
  if (players_.back()->GetId() == 0) {
    players_.back()->SetType(Player::Type::kDefender);
  } else {
    players_.back()->SetType(Player::Type::kAttacker);
  }
  connect(players_.back()->Socket(),
          &QTcpSocket::readyRead,
          this,
          &Server::ReceiveClientData);
  connect(players_.back()->Socket(),
          &QTcpSocket::disconnected,
          this,
          &Server::DisconnectClient);
  UpdateClientsInfo();
}

void Server::DisconnectClient() {
  for (size_t i = 0; i < players_.size(); i++) {
    if (players_[i]->Socket()->state() == QAbstractSocket::UnconnectedState) {
      players_.erase(players_.begin() + i);
      if (defender_data_.size() != 0) {
        defender_data_.clear();
      }
      if (attacker_data_.size() != 0) {
        attacker_data_.clear();
      }
    }
  }
  for (size_t i = 0; i < players_.size(); i++) {
    players_[i]->SetId(i);
  }
  if (defender_data_.size() == 0 && attacker_data_.size() == 0) {
    killTimer(timer_id_);
    timer_id_ = -1;
  }
  UpdateClientsInfo();
}

void Server::timerEvent(QTimerEvent*) {
  SendGameStateToAllPlayers();
}

void Server::ReceiveClientData() {
  for (auto& player: players_) {
    if (player->Socket()->bytesAvailable()) {
      QByteArray arr = player->Socket()->readAll();
      QDataStream data_stream(&arr, QIODevice::ReadOnly);
      NetworkData data;
      data_stream >> data.type;
      data_stream >> data.data;
      switch (data.type) {
        case MessageType::kReadyStatus: {
          size_t id = data.data.toInt();
          players_[id]->SetReady(!players_[id]->IsReady());
          UpdateClientsInfo();
          break;
        }
        case MessageType::kEndPreparationStatus: {
          size_t id = data.data.toInt();
          players_[id]->SetPrepared(true);
          if (IsAllPrepared()) {
            SendActiveStageSignal(data.data);
          }
          break;
        }
        case MessageType::kSignalToStart: {
          defender_data_.clear();
          attacker_data_.clear();
          SendStartSignal(data.data);
          break;
        }
        case MessageType::kPlayersData: {
          if (player->GetType() == Player::Type::kDefender) {
            defender_data_ = data.data.toString();
          } else {
            attacker_data_ = data.data.toString();
          }
          break;
        }
        case MessageType::kPlayersVector: {
          break;
        }
      }
    }
  }
}

void Server::UpdateClientsInfo() {
  Network::WriteDataForAll(players_,
                           QVariant::fromValue(JsonHelper::EncodePlayersVector(players_)),
                           MessageType::kPlayersVector);
}

void Server::SendStartSignal(const QVariant& q_variant) {
  Network::WriteDataForAll(players_,
                           q_variant,
                           MessageType::kSignalToStart);
  defender_data_.clear();
  attacker_data_.clear();
}

void Server::SendActiveStageSignal(const QVariant& q_variant) {
  Network::WriteDataForAll(players_,
                           q_variant,
                           MessageType::kEndPreparationStatus);
  if (timer_id_ == -1) {
    timer_id_ = startTimer(Network::kMillisDataSend);
  }
}

void Server::SendGameStateToAllPlayers() {
  for (auto& player: players_) {
    if (player->GetType() == Player::Type::kDefender) {
      Network::WriteData(player->Socket(),
                         QVariant::fromValue(attacker_data_),
                         MessageType::kPlayersData);
    } else {
      Network::WriteData(player->Socket(),
                         QVariant::fromValue(defender_data_),
                         MessageType::kPlayersData);
    }
  }
}

bool Server::IsAllPrepared() {
  for (const auto& player: players_) {
    if (!player->IsPrepared()) {
      return false;
    }
  }
  return true;
}

#include "server.h"

#include <string>

#include <QFile>
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
  players_.emplace_back(
      std::make_shared<Player>(server_.nextPendingConnection()));
  players_.back()->SetId(players_.size() - 1);
  if (players_.back()->GetId() == 0) {
    players_.back()->SetSide(Side::kAttacker);
  } else {
    players_.back()->SetSide(Side::kDefender);
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
      defender_data_.clear();
      attacker_data_.clear();
    }
  }
  for (size_t i = 0; i < players_.size(); i++) {
    players_[i]->SetId(i);
  }
  UpdateClientsInfo();
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
          break;
        }
        case MessageType::kSignalToStart: {
          defender_data_.clear();
          attacker_data_.clear();
          SendStartSignal(data.data);
          break;
        }
        case MessageType::kPlayersData: {
          if (player->GetSide() == Side::kDefender) {
            defender_data_ = data.data;
          } else {
            attacker_data_ = data.data;
          }
          players_[player->GetId()]->SetPrepared(true);
          if (IsAllPrepared()) {
            SendGameStateToAllPlayers();
          }
          break;
        }
        default: {
          break;
        }
      }
    }
  }
}

void Server::UpdateClientsInfo() {
  Network::WriteDataForAll(
      players_,
      QVariant::fromValue(JsonHelper::EncodePlayerData(players_)),
      MessageType::kPlayersVector);
}

void Server::SendStartSignal(const QVariant& q_variant) {
  Network::WriteDataForAll(players_,
                           q_variant,
                           MessageType::kSignalToStart);
  defender_data_.clear();
  attacker_data_.clear();
}

void Server::SendGameStateToAllPlayers() {
  for (auto& player: players_) {
    if (player->GetSide() == Side::kDefender) {
      Network::WriteData(player->Socket(),
                         QVariant::fromValue(attacker_data_),
                         MessageType::kEndPreparationStatus);
    } else {
      Network::WriteData(player->Socket(),
                         QVariant::fromValue(defender_data_),
                         MessageType::kEndPreparationStatus);
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

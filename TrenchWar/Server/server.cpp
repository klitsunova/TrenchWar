#include "server.h"

#include <QTcpSocket>
#include <QNetworkInterface>

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
  for (const auto& address : addresses) {
    ip_addresses += address.toString();
    ip_addresses += '\n';
  }
  ip_.setText(ip_addresses);
}

void Server::ConnectClient() {
  players_.emplace_back(server_.nextPendingConnection());
  players_.back().SetId(players_.size() - 1);
  connect(players_.back().Socket(),
          &QTcpSocket::readyRead,
          this,
          &Server::ReceiveClientData);
  connect(players_.back().Socket(),
          &QTcpSocket::disconnected,
          this,
          &Server::DisconnectClient);
  UpdateClientsInfo();
}

void Server::DisconnectClient() {
  for (size_t i = 0; i < players_.size(); i++) {
    if (players_[i].Socket()->state() == QAbstractSocket::UnconnectedState) {
      players_.erase(players_.begin() + i);
      if (!players_data_.empty()) {
        players_data_.erase(players_data_.begin() + i);
      }
    }
  }
  for (size_t i = 0; i < players_.size(); i++) {
    players_[i].SetId(i);
  }
  if (players_data_.empty()) {
    killTimer(timer_id_);
    timer_id_ = -1;
  }
  UpdateClientsInfo();
}

void Server::timerEvent(QTimerEvent*) {
  SendGameStateToAllPlayers();
}

void Server::ReceiveClientData() {
  for (auto& player : players_) {
    if (player.Socket()->bytesAvailable()) {
      QByteArray arr = player.Socket()->readAll();
      QDataStream data_stream(&arr, QIODevice::ReadOnly);
      NetworkData data;
      data_stream >> data.type;
      data_stream >> data.data;
      switch (data.type) {
        case MessageType::kReadyStatus : {
          size_t id = data.data.toInt();
          players_[id].SetReady(!players_[id].IsReady());
          UpdateClientsInfo();
          break;
        }
        case MessageType::kSignalToStart : {
          players_data_.clear();
          SendStartSignal(data.data);
          break;
        }
        case MessageType::kPlayersData : {
          // players_data_[player.GetId()] =
          //     JsonHelper::DecodePlayersData(data.data.toString());
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
  Network::WriteDataForAll(
      &players_,
      QVariant::fromValue(JsonHelper::EncodePlayersVectorJson(players_)),
      MessageType::kPlayersVector);
}

void Server::SendStartSignal(const QVariant& q_variant) {
  Network::WriteDataForAll(&players_,
                           q_variant,
                           MessageType::kSignalToStart);
  if (timer_id_ == -1) {
    timer_id_ = startTimer(Network::kMillisDataSend);
  }
  players_data_.clear();
  players_data_.resize(players_.size());
}

void Server::SendGameStateToAllPlayers() {
  // QString json = JsonHelper::EncodeServerPlayersData(players_cars_data_);
  // Network::WriteDataForAll(&players_,
  //                          QVariant::fromValue(json),
  //                          MessageType::kPlayersData);
}

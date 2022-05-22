#include "network_controller.h"

#include <QDataStream>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>

NetworkController::NetworkController(Player* player) : player_(player) {
  connect(player_->Socket(),
          &QTcpSocket::readyRead,
          this,
          &NetworkController::ParseData);
}

void NetworkController::SendReadyStatus() {
  Network::WriteData(player_->Socket(),
                     QVariant::fromValue(player_->GetId()),
                     MessageType::kReadyStatus);
}

void NetworkController::ParseData() {
  QByteArray arr = player_->Socket()->readAll();
  QDataStream data_stream(&arr, QIODevice::ReadOnly);
  NetworkData data;
  data_stream >> data.type;
  data_stream >> data.data;
  switch (data.type) {
    case MessageType::kPlayersVector: {
      q_variant_ = data.data;
      emit GotPlayersVector();
      break;
    }
    case MessageType::kSignalToStart: {
      q_variant_ = data.data;
      emit GotSignalToStart();
      break;
    }
    case MessageType::kPlayersData: {
      DecodePlayersData(data.data);
      break;
    }
    default: {
      break;
    }
  }
}

void NetworkController::DecodePlayersData(const QVariant& q_variant) {
}

QVariant NetworkController::GetData() {
  return q_variant_;
}

void NetworkController::SendStartSignal(const QString& json) {
  Network::WriteData(player_->Socket(),
                     QVariant::fromValue(json),
                     MessageType::kSignalToStart);
}

bool NetworkController::DataUpdated(size_t id) {
  // if (players_data_[id].is_updated) {
  //   players_data_[id].is_updated = false;
  //   return true;
  // } else {
  return false;
  // }
}

PlayerData NetworkController::GetPlayersData(size_t id) {
  return players_data_[id];
}

void NetworkController::SendData(PlayerData data) {
  // Network::WriteData(player_->Socket(),
  //                    QVariant::fromValue(JsonHelper::EncodePlayerCarData(data)),
  //                    MessageType::kPlayersCarData);
}

size_t NetworkController::GetId() {
  return player_->GetId();
}

bool NetworkController::IsStarted() const {
  return is_started_;
}

void NetworkController::SetStarted(bool is_started) {
  is_started_ = is_started;
}

#include "network_controller.h"

#include <QDataStream>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>

#include <QMessageBox>
#include <utility>

NetworkController::NetworkController(std::shared_ptr<Player> player) : player_(std::move(player)) {
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

void NetworkController::SendEndPreparationStatus() {
  Network::WriteData(player_->Socket(),
                     QVariant::fromValue(player_->GetId()),
                     MessageType::kEndPreparationStatus);
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
    case MessageType::kEndPreparationStatus: {
      q_variant_ = data.data;
      is_started_ = true;
      emit GotSignalForActiveStage();
      break;
    }
    case MessageType::kPlayersData: {
      enemy_data_ = JsonHelper::DecodePlayerData(data.data);
      break;
    }
    default: {
      break;
    }
  }
}

QVariant NetworkController::GetData() {
  return q_variant_;
}

void NetworkController::SendStartSignal() {
  Network::WriteData(player_->Socket(),
                     QVariant(),
                     MessageType::kSignalToStart);
}

// bool NetworkController::DataUpdated(size_t id) {
//   if (player_->IsDataUpdated()) {
//     player_->SetDataUpdated(false);
//     return true;
//   } else {
//     return false;
//   }
// }

void NetworkController::SendData() {
  Network::WriteData(player_->Socket(),
                     QVariant::fromValue(JsonHelper::EncodePlayerData(own_data_)),
                     MessageType::kPlayersData);
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

PlayerData& NetworkController::GetEnemyData() {
  return enemy_data_;
}

void NetworkController::SetOwnData(PlayerData data) {
  own_data_ = std::move(data);
}

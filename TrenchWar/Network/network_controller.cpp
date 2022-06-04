#include "network_controller.h"

#include <QDataStream>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>

#include <QMessageBox>
#include <utility>

NetworkController::NetworkController(std::shared_ptr<Player> player)
    : player_(std::move(player)) {
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
      if (player_->GetSide() == Side::kAttacker) {
        defenders_data_ = JsonHelper::DecodeGameData(data.data);

      } else {
        attackers_data_ = JsonHelper::DecodeGameData(data.data);
      }
      is_started_ = true;
      emit GotSignalForActiveStage();
      break;
    }
    case MessageType::kPlayersData:
    case MessageType::kReadyStatus:
      break;
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

void NetworkController::SendData() {
  if (player_->GetSide() == Side::kAttacker) {
    Network::WriteData(
        player_->Socket(),
        QVariant::fromValue(JsonHelper::EncodeGameData(attackers_data_)),
        MessageType::kPlayersData);
  } else {
    Network::WriteData(
        player_->Socket(),
        QVariant::fromValue(JsonHelper::EncodeGameData(defenders_data_)),
        MessageType::kPlayersData);
  }
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

void NetworkController::SetAttackersData(GameData data) {
  attackers_data_ = std::move(data);
}

void NetworkController::SetDefendersData(GameData data) {
  defenders_data_ = std::move(data);
}

GameData NetworkController::GetAttackersData() const {
  return attackers_data_;
}

GameData NetworkController::GetDefendersData() const {
  return defenders_data_;
}

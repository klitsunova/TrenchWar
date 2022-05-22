#pragma once

#include <vector>

#include <QTcpSocket>
#include <QVariant>

#include "Network/player_model.h"

enum class MessageType {
  kPlayersVector,
  kReadyStatus,
  kSignalToStart,
  kPlayersData,
};

struct NetworkData {
  QVariant data;
  MessageType type;
};

struct PlayerData {
};

class Network {
 public:
  static constexpr int kPort = 5555;
  static constexpr int kMillisDataSend = 20;

  static void WriteData(
      QTcpSocket* socket,
      const QVariant& q_variant,
      MessageType type);

  static void WriteDataForAll(
      std::vector<Player>* players_,
      const QVariant& q_variant,
      MessageType type);

 private:
  static QByteArray GetDataBytes(
      const QVariant& q_variant,
      const MessageType& type);
};

class JsonHelper {
 public:
  static QString EncodePlayersVectorJson(const std::vector<Player>& players);
  static std::vector<std::pair<size_t, bool>> DecodePlayersVectorJson(
      const QString& json);
};
#pragma once

#include <vector>

#include <QTcpSocket>
#include <QVariant>

#include "Network/player_model.h"
#include "Models/GameObjects/soldier.h"
#include "Models/GameObjects/bullet.h"

enum class MessageType {
  kPlayersVector,
  kReadyStatus,
  kSignalToStart,
  kPlayersData,
  kEndPreparationStatus,
};

struct NetworkData {
  QVariant data;
  MessageType type;
};

struct PlayerData {
  std::vector<Soldier> soldiers;
  std::vector<Bullet> bullets;
  bool is_updated;
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
      const std::vector<std::shared_ptr<Player>>& players_,
      const QVariant& q_variant,
      MessageType type);

 private:
  static QByteArray GetDataBytes(
      const QVariant& q_variant,
      const MessageType& type);
};

class JsonHelper {
 public:
  static QString EncodePlayersVector(const std::vector<std::shared_ptr<Player>>& players);
  static std::vector<std::pair<size_t, bool>> DecodePlayersVectorJson(
      const QString& json);
  static QString EncodePlayerData(const PlayerData& data);
  static PlayerData DecodePlayerData(const QVariant& q_variant);
};
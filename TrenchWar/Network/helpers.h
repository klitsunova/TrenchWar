#pragma once

#include <memory>
#include <utility>
#include <vector>

#include <QTcpSocket>
#include <QVariant>

#include "Models/GameObjects/bullet.h"
#include "Models/GameObjects/soldier.h"
#include "Network/player_model.h"

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
  int id;
  bool status;
  int side;
};

struct SoldierData {
  int x;
  int y;
  int hit_points;
};

struct GameData {
  std::vector<SoldierData> soldiers;
  std::vector<std::pair<int, int>> trenches;
  std::vector<std::pair<int, int>> terrain_objects;
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
  static QString EncodePlayerData(
      const std::vector<std::shared_ptr<Player>>& players);
  static std::vector<PlayerData> DecodePlayersData(const QString& json);
  static QVariant EncodeGameData(const GameData& data);
  static GameData DecodeGameData(const QVariant& q_variant);
};

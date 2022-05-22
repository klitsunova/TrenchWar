#pragma once

#include <vector>

#include <QTcpSocket>

#include "helpers.h"
#include "player_model.h"

class NetworkController : public QObject {
  Q_OBJECT

 public:
  explicit NetworkController(Player* player);

  void SendReadyStatus();
  void SendStartSignal(const QString& json);
  bool DataUpdated(size_t id);

  QVariant GetData();
  PlayerData GetPlayersData(size_t id);
  void SendData(PlayerData player_data);

  size_t GetId();
  bool IsStarted() const;
  void SetStarted(bool is_started);

 signals:
  void StartGame();
  void GotPlayersVector();
  void GotSignalToStart();

 private:
  void ParseData();
  void DecodePlayersData(const QVariant& q_variant);

  Player* player_;
  QVariant q_variant_;
  std::vector<PlayerData> players_data_;
  bool is_started_ = false;
};
#pragma once

#include <vector>

#include <QTcpSocket>

#include "helpers.h"
#include "player_model.h"

class NetworkController : public QObject {
  Q_OBJECT

 public:
  explicit NetworkController(std::shared_ptr<Player> player);

  void SendReadyStatus();
  void SendStartSignal();
  void SendEndPreparationStatus();

  QVariant GetData();
  void SendData();
  PlayerData& GetEnemyData();
  void SetOwnData(PlayerData data);

  size_t GetId();
  bool IsStarted() const;
  void SetStarted(bool is_started);

 signals:
  void StartGame();
  void GotPlayersVector();
  void GotSignalToStart();
  void GotSignalForActiveStage();

 private:
  void ParseData();

  std::shared_ptr<Player> player_;
  QVariant q_variant_;
  // std::vector<PlayerData> players_data_;
  bool is_started_ = false;
  PlayerData own_data_;
  PlayerData enemy_data_;
};
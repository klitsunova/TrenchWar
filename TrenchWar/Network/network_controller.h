#pragma once

#include <memory>
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

  void SetAttackersData(GameData data);
  void SetDefendersData(GameData data);

  GameData GetAttackersData() const;
  GameData GetDefendersData() const;

  GameData& GetEnemyData();
  void SetOwnData(GameData data);

  Side GetPlayerSide() const;

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
  bool is_started_ = false;
  GameData defenders_data_;
  GameData attackers_data_;
};

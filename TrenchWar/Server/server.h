#pragma once

#include <vector>

#include <QLabel>
#include <QTcpServer>
#include <QTimerEvent>
#include <QWidget>

#include "Network/helpers.h"
#include "Network/player_model.h"

class Server : public QWidget {
  Q_OBJECT

 public:
  Server();

  void timerEvent(QTimerEvent*) override;

 private:
  std::vector<std::shared_ptr<Player>> players_;
  QString defender_data_;
  QString attacker_data_;
  QLabel ip_;
  QTcpServer server_;
  int timer_id_ = -1;

  void ConnectClient();
  void DisconnectClient();
  void ReceiveClientData();
  void UpdateClientsInfo();

  bool IsAllPrepared();

  void SendStartSignal(const QVariant& q_variant);
  void SendActiveStageSignal(const QVariant& q_variant);

  void SendGameStateToAllPlayers();
  void ShowIp();
};

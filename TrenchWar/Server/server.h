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

 private:
  std::vector<std::shared_ptr<Player>> players_;
  QVariant defender_data_;
  QVariant attacker_data_;
  QLabel ip_;
  QTcpServer server_;

  void ConnectClient();
  void DisconnectClient();
  void ReceiveClientData();
  void UpdateClientsInfo();
  void SendStartSignal(const QVariant& q_variant);
  void SendGameStateToAllPlayers();

  bool IsAllPrepared();
  void ShowIp();
};

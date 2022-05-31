#pragma once

#include <utility>
#include <vector>

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include "player_model.h"
#include "player_view.h"
#include "network_controller.h"

class NetworkView : public QWidget {
  Q_OBJECT

 public:
  explicit NetworkView(QWidget* parent);
  ~NetworkView() override = default;
  void Disconnect();

  void SetEndPreparationStatus();

  std::shared_ptr<NetworkController> GetNetworkController() const;
  Side GetPlayerSide() const;

 signals:
  void StartGame();
  void StartActiveStage();
  void ReturnToMainMenu();
  void ExitDisconnected();

 private:
  static constexpr int kMillisWaitForConnection = 100;

  void UpdatePlayersVector();
  void SetUpAndStartGame();
  void PrepareForStart();
  void ChangeReadyStatus();
  void Connect();

  void SetUpLayouts();
  void SetStyles();
  void ConnectButtons() const;
  void AddStartButton();

  QPushButton* back_to_main_menu_;
  QPushButton* try_connect_;
  QPushButton* ready_;
  QPushButton* disconnect_;
  QPushButton* start_button_ = nullptr;
  QLineEdit* ip_;
  QLabel* connection_status_;
  QVBoxLayout* main_layout_;
  QVBoxLayout* connection_layout_;
  QHBoxLayout* buttons_layout_;
  QVBoxLayout* players_layout_;

  bool is_first_packet_received_ = false;
  std::shared_ptr<Player> network_player_ = nullptr;
  std::vector<PlayerView*> players_;
  std::shared_ptr<NetworkController> network_controller_ = nullptr;
};

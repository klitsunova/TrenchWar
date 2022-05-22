#pragma once

#include <QTcpSocket>

class Player {
 public:
  explicit Player(QTcpSocket* p_socket);

  QTcpSocket* Socket();

  size_t GetId() const;
  void SetId(size_t id);

  bool IsReady() const;
  void SetReady(bool is_ready);

 private:
  size_t id_;
  QTcpSocket* socket_;
  bool is_ready_ = false;
};

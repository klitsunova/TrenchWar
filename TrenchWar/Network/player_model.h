#pragma once

#include <QTcpSocket>

#include "helpers/enum_helpers.h"

class Player {
 public:
  explicit Player(QTcpSocket* p_socket);

  QTcpSocket* Socket();

  size_t GetId() const;
  void SetId(size_t id);

  bool IsReady() const;
  void SetReady(bool is_ready);

  bool IsPrepared() const;
  void SetPrepared(bool is_prepared);

  Side GetSide() const;
  void SetSide(Side side);

  bool IsDataUpdated() const;
  void SetDataUpdated(bool state);

 private:
  size_t id_;

  QTcpSocket* socket_;
  bool is_ready_ = false;
  bool is_prepared_ = false;
  Side side_;
};

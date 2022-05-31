#include "player_model.h"

Player::Player(QTcpSocket* p_socket) : socket_(p_socket) {
}

QTcpSocket* Player::Socket() {
  return socket_;
}

size_t Player::GetId() const {
  return id_;
}

void Player::SetId(size_t id) {
  id_ = id;
}

bool Player::IsReady() const {
  return is_ready_;
}

void Player::SetReady(bool is_ready) {
  is_ready_ = is_ready;
}

bool Player::IsPrepared() const {
  return is_prepared_;
}

void Player::SetPrepared(bool is_prepared) {
  is_prepared_ = is_prepared;
}

Side Player::GetSide() const {
  return side_;
}

void Player::SetSide(Side side) {
  side_ = side;
}

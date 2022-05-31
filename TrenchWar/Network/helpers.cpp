#include "helpers.h"

#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QtCore>
#include <iostream>

void Network::WriteData(QTcpSocket* socket,
                        const QVariant& q_variant,
                        MessageType type) {
  socket->write(GetDataBytes(q_variant, type));
}

void Network::WriteDataForAll(const std::vector<std::shared_ptr<Player>>& players_,
                              const QVariant& q_variant,
                              MessageType type) {
  for (auto& player: players_) {
    player->Socket()->write(GetDataBytes(q_variant, type));
  }
}

QByteArray Network::GetDataBytes(const QVariant& q_variant,
                                 const MessageType& type) {
  NetworkData data;
  data.type = type;
  data.data = q_variant;
  QByteArray arr;
  QDataStream data_stream(&arr, QIODevice::WriteOnly);
  data_stream << data.type << data.data;
  return arr;
}

QString JsonHelper::EncodePlayerData(
    const std::vector<std::shared_ptr<Player>>& players) {
  QJsonObject json_object;
  QJsonArray array;
  for (size_t i = 0; i < players.size(); i++) {
    QJsonObject record;
    record.insert("id", QJsonValue::fromVariant(static_cast<int>(i)));
    record.insert("status", QJsonValue::fromVariant(players[i]->IsReady()));
    record.insert("side", QJsonValue::fromVariant(static_cast<int>(players[i]->GetSide())));
    array.push_back(record);
  }
  json_object.insert("data", array);
  return QJsonDocument(json_object).toJson();
}

std::vector<PlayerData> JsonHelper::DecodePlayersData(
    const QString& json) {
  QJsonObject json_object = QJsonDocument::fromJson(json.toUtf8()).object();
  QJsonArray data_array = json_object["data"].toArray();
  std::vector<PlayerData> result;
  for (const auto& data: data_array) {
    QJsonObject data_obj = data.toObject();
    result.emplace_back(PlayerData{data_obj["id"].toInt(),
                                   data_obj["status"].toBool(),
                                   data_obj["side"].toInt()});
  }
  return result;
}

QString JsonHelper::EncodeGameData(const GameData& data) {
  QJsonObject json_object;
  QJsonArray json_soldier_array;
  for (auto& soldier: data.soldiers) {
    QJsonObject json_soldier;
    json_soldier.insert("x", QJsonValue::fromVariant(soldier.x));
    json_soldier.insert("y", QJsonValue::fromVariant(soldier.y));
    json_soldier.insert("hit_points", QJsonValue::fromVariant(soldier.hit_points));
    json_soldier_array.push_back(json_soldier);
  }
  json_object.insert("soldiers", json_soldier_array);
  QJsonArray json_bullet_array;
  for (auto& bullet: data.bullets) {
    QJsonObject json_bullet;
    json_bullet.insert("from_x", QJsonValue::fromVariant(bullet.GetFromPosition().x()));
    json_bullet.insert("from_y", QJsonValue::fromVariant(bullet.GetFromPosition().y()));
    json_bullet.insert("to_x", QJsonValue::fromVariant(bullet.GetToPosition().x()));
    json_bullet.insert("to_y", QJsonValue::fromVariant(bullet.GetToPosition().y()));
    json_bullet.insert("side", QJsonValue::fromVariant(static_cast<int>(bullet.GetSide())));
    json_bullet.insert("damage", QJsonValue::fromVariant(bullet.GetDamage()));
    json_bullet_array.push_back(json_bullet);
  }
  return QJsonDocument(json_object).toJson();
}

GameData JsonHelper::DecodeGameData(const QVariant& q_variant) {
  QString json = q_variant.toString();
  GameData new_data;
  QJsonObject json_object = QJsonDocument::fromJson(json.toUtf8()).object();
  QJsonArray soldiers_data = json_object["soldiers"].toArray();
  for (const auto& record: soldiers_data) {
    QJsonObject json_soldier = record.toObject();
    SoldierData soldier{
        json_soldier["x"].toInt(),
        json_soldier["y"].toInt(),
        json_soldier["hit_points"].toInt()};
    new_data.soldiers.push_back(soldier);
  }
  QJsonArray bullets_data = json_object["bullets"].toArray();
  for (const auto& record: bullets_data) {
    QJsonObject json_bullet = record.toObject();
    QPoint position_from = QPoint(json_bullet["from_x"].toInt(),
                                  json_bullet["from_y"].toInt());
    QPoint position_to = QPoint(json_bullet["to_x"].toInt(),
                                json_bullet["to_y"].toInt());
    Side side = static_cast<Side>(json_bullet["side"].toInt());
    new_data.bullets.emplace_back(position_from,
                                  position_to,
                                  side,
                                  json_bullet["damage"].toInt());
  }
  return new_data;
}

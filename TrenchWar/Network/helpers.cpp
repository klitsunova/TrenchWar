#include "helpers.h"

#include <memory>

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

void Network::WriteDataForAll(
    const std::vector<std::shared_ptr<Player>>& players_,
    const QVariant& q_variant,
    MessageType type) {
  for (auto& player : players_) {
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
    int side = static_cast<int>(players[i]->GetSide());
    record.insert("id", QJsonValue::fromVariant(static_cast<int>(i)));
    record.insert("status", QJsonValue::fromVariant(players[i]->IsReady()));
    record.insert("side", QJsonValue::fromVariant(side));
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
  for (const auto& data : data_array) {
    QJsonObject data_obj = data.toObject();
    result.emplace_back(PlayerData{data_obj["id"].toInt(),
                                   data_obj["status"].toBool(),
                                   data_obj["side"].toInt()});
  }
  return result;
}

QVariant JsonHelper::EncodeGameData(const GameData& data) {
  QJsonObject json_object;

  QJsonArray json_soldier_array;
  for (auto& soldier : data.soldiers) {
    QJsonObject json_soldier;
    json_soldier.insert("x", QJsonValue::fromVariant(soldier.x));
    json_soldier.insert("y", QJsonValue::fromVariant(soldier.y));
    json_soldier.insert("hit_points",
                        QJsonValue::fromVariant(soldier.hit_points));
    json_soldier_array.push_back(json_soldier);
  }
  json_object.insert("soldiers", json_soldier_array);

  QJsonArray json_trench_array;
  for (auto& trench : data.trenches) {
    QJsonObject json_trench;
    json_trench.insert("x", QJsonValue::fromVariant(trench.first));
    json_trench.insert("y", QJsonValue::fromVariant(trench.second));
    json_trench_array.push_back(json_trench);
  }
  json_object.insert("trenches", json_trench_array);
  return json_object;
}

GameData JsonHelper::DecodeGameData(const QVariant& q_variant) {
  GameData new_data;
  QJsonObject json_object = q_variant.toJsonObject();
  QJsonArray soldiers_data = json_object["soldiers"].toArray();
  for (const auto& record : soldiers_data) {
    QJsonObject json_soldier = record.toObject();
    new_data.soldiers.push_back(
        {json_soldier["x"].toInt(),
         json_soldier["y"].toInt(),
         json_soldier["hit_points"].toInt()});
  }
  QJsonArray trench_data = json_object["trenches"].toArray();
  for (const auto& record : trench_data) {
    QJsonObject json_trench = record.toObject();
    new_data.trenches.emplace_back(
        json_trench["x"].toInt(),
        json_trench["y"].toInt());
  }
  return new_data;
}

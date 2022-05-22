#include "helpers.h"

#include <utility>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QFile>
#include <QtCore>
#include <iostream>
#include <QIODevice>

void Network::WriteData(QTcpSocket* socket,
                        const QVariant& q_variant,
                        MessageType type) {
  socket->write(GetDataBytes(q_variant, type));
}

void Network::WriteDataForAll(const std::vector<std::shared_ptr<Player>>& players_,
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

QString JsonHelper::EncodePlayersVector(
    const std::vector<std::shared_ptr<Player>>& players) {
  QJsonObject json_object;
  QJsonArray array;
  for (size_t i = 0; i < players.size(); i++) {
    QJsonObject record;
    record.insert("id", QJsonValue::fromVariant(static_cast<int>(i)));
    record.insert("status", QJsonValue::fromVariant(players[i]->IsReady()));
    array.push_back(record);
  }
  json_object.insert("data", array);
  return QJsonDocument(json_object).toJson();
}

std::vector<std::pair<size_t, bool>> JsonHelper::DecodePlayersVectorJson(
    const QString& json) {
  QJsonObject json_object = QJsonDocument::fromJson(json.toUtf8()).object();
  QJsonArray data_array = json_object["data"].toArray();
  std::vector<std::pair<size_t, bool>> result;
  for (const auto& data: data_array) {
    QJsonObject data_obj = data.toObject();
    result.emplace_back(data_obj["id"].toInt(),
                        data_obj["status"].toBool());
  }
  return result;
}

QString JsonHelper::EncodePlayerData(const PlayerData& data) {
  QJsonObject json_object;
  QJsonArray json_soldier_array;
  for (auto& soldier : data.soldiers) {
    QJsonObject json_soldier;
    json_soldier.insert("id", QJsonValue::fromVariant(QVariant::fromValue(soldier.GetId())));
    json_soldier.insert("x", QJsonValue::fromVariant(soldier.GetPosition().x()));
    json_soldier.insert("y", QJsonValue::fromVariant(soldier.GetPosition().y()));
    json_soldier_array.push_back(json_soldier);
  }
  json_object.insert("soldiers", json_soldier_array);
  QJsonDocument doc;
  doc.setObject(json_object);
  QByteArray bytes = doc.toJson( QJsonDocument::Indented );
  QFile file("text.txt");
  if( file.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate ) )
  {
    QTextStream iStream( &file );
    // iStream.setCodec( "utf-8" );
    iStream << bytes;
    file.close();
  }

  return QJsonDocument(json_object).toJson();
}

PlayerData JsonHelper::DecodePlayerData(const QVariant& q_variant) {
  QString json = q_variant.toString();
  PlayerData new_data;
  QJsonObject json_object = QJsonDocument::fromJson(json.toUtf8()).object();
  QJsonArray soldiers_data = json_object["soldiers"].toArray();
  for (const auto& record : soldiers_data) {
    QJsonObject soldier_obj = record.toObject();
    Soldier soldier(Soldier::Type::kAttacker);
    soldier.SetId(soldier_obj["id"].toInt());
    soldier.SetPosition(QPoint(soldier_obj["x"].toInt(), soldier_obj["y"].toInt()));
    new_data.soldiers.push_back(soldier);
  }
  return new_data;
}

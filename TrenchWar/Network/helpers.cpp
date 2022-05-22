#include "helpers.h"

#include <utility>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

void Network::WriteData(QTcpSocket* socket,
                        const QVariant& q_variant,
                        MessageType type) {
  socket->write(GetDataBytes(q_variant, type));
}

void Network::WriteDataForAll(std::vector<Player>* players_,
                              const QVariant& q_variant,
                              MessageType type) {
  for (auto& player: *players_) {
    player.Socket()->write(GetDataBytes(q_variant, type));
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

QString JsonHelper::EncodePlayersVectorJson(
    const std::vector<Player>& players) {
  QJsonObject json_object;
  QJsonArray array;
  for (size_t i = 0; i < players.size(); i++) {
    QJsonObject record;
    record.insert("id", QJsonValue::fromVariant(static_cast<int>(i)));
    record.insert("status", QJsonValue::fromVariant(players[i].IsReady()));
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

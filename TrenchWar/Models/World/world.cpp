#include <random>
#include <utility>

#include "world.h"

#include "Models/Tools/settings.h"

#include <QAudioOutput>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMediaPlayer>

World::World(const QString& path, GameMode mode, Side side) {
  LoadMap(path, mode, side);
}

void World::AddSoldier(Side side) {
  QRect field({0, 0}, GetSize());
  AddSoldier(side, utils::RandomGenerator::GetRandomPoint(QRect(field)));
}

void World::AddSoldier(Side side, const QPoint& position) {
  assert(position.y() >= 0 && position.y() < cells_.size());
  assert(position.x() >= 0 && position.x() < cells_[position.y()].size());
  auto new_object = std::make_shared<Soldier>(side, position);
  auto& cell =
      cells_[new_object->GetPosition().y()][new_object->GetPosition().x()];
  soldiers_.push_back(new_object);
  cell.InsertSoldier(new_object);
}

void World::AddTower() {
  QRect field({0, 0}, GetSize());
  AddTower(utils::RandomGenerator::GetRandomPoint(QRect(field)));
}

void World::AddTower(const QPoint& position) {
  auto new_object = std::make_shared<Tower>(position);
  towers_.push_back(new_object);
  distances_map_.AddObject(new_object);
}

void World::AddBullet(const std::shared_ptr<Bullet>& bullet) {
  auto* player = new QMediaPlayer(this);
  auto* audioOutput = new QAudioOutput(this);
  player->setAudioOutput(audioOutput);
  audioOutput->setVolume(Settings::GetMusicVolume() /
      static_cast<double>(Settings::kMaxVolume - Settings::kMinVolume));
  player->setSource(QUrl("qrc:Resources/Music/singleshot_voice.mp3"));
  player->play();
  assert(bullet.get() != nullptr);
  bullets_.push_back(bullet);
}

const std::list<std::shared_ptr<Soldier>>& World::GetSoldiers() const {
  return soldiers_;
}

const std::list<std::shared_ptr<Tower>>& World::GetTowers() const {
  return towers_;
}

const std::list<std::shared_ptr<Bullet>>& World::GetBullets() const {
  return bullets_;
}

const QSize& World::GetSize() const {
  return distances_map_.GetSize();
}

const QPixmap& World::GetPixmap() {
  return distances_map_.GetPixmap();
}

void World::MoveSoldiers() {
  if (towers_.empty()) return;

  auto MoveIf = [&](const std::shared_ptr<Soldier>& soldier,
                    int& current_dist,
                    const QPoint& to, int lag = 0) {
    int new_dist = GetDistance(to) + lag;
    if (current_dist > new_dist) {
      current_dist = new_dist;
      soldier->SetPosition(to);
      soldier->SetTimeLag(
          cells_[to.y()][to.x()].GetTimeLag() + lag);
    }
  };

  enum class Command {
    MoveLeftDown,
    MoveLeftUp,
    MoveRightUp,
    MoveRightDown,
    MoveLeft,
    MoveRight,
    MoveUp,
    MoveDown
  };

  auto IssueCommand = [&](const std::shared_ptr<Soldier>& soldier,
                          const QPoint& from,
                          int& current_dist, Command command) {
    switch (command) {
      case Command::MoveLeft: {
        if (from.x() == 0) return;

        return MoveIf(soldier, current_dist, QPoint(from.x() - 1, from.y()));
      }
      case Command::MoveRight: {
        if (from.x() == cells_[from.y()].size() - 1) return;

        return MoveIf(soldier, current_dist, QPoint(from.x() + 1, from.y()));
      }
      case Command::MoveUp: {
        if (from.y() == 0) return;

        return MoveIf(soldier, current_dist, QPoint(from.x(), from.y() - 1));
      }
      case Command::MoveDown: {
        if (from.y() == cells_.size() - 1) return;

        return MoveIf(soldier, current_dist, QPoint(from.x(), from.y() + 1));
      }
      case Command::MoveLeftDown: {
        if (from.x() == 0 || from.y() == cells_.size() - 1) return;

        int lag = std::min(GetLag(QPoint(from.x() - 1, from.y())),
                           GetLag(QPoint(from.x(), from.y() + 1)));
        return MoveIf(soldier, current_dist,
                      QPoint(from.x() - 1, from.y() + 1), lag);
      }
      case Command::MoveLeftUp: {
        if (from.x() == 0 || from.y() == 0) return;

        int lag = std::min(GetLag(QPoint(from.x() - 1, from.y())),
                           GetLag(QPoint(from.x(), from.y() - 1)));
        return MoveIf(soldier, current_dist,
                      QPoint(from.x() - 1, from.y() - 1), lag);
      }
      case Command::MoveRightUp: {
        if (from.x() == cells_[from.y()].size() - 1 || from.y() == 0) return;

        int lag = std::min(GetLag(QPoint(from.x() + 1, from.y())),
                           GetLag(QPoint(from.x(), from.y() - 1)));
        return MoveIf(soldier, current_dist,
                      QPoint(from.x() + 1, from.y() - 1), lag);
      }
      case Command::MoveRightDown: {
        if (from.x() == cells_[from.y()].size() - 1
            || from.y() == cells_.size() - 1)
          return;

        int lag = std::min(GetLag(QPoint(from.x() + 1, from.y())),
                           GetLag(QPoint(from.x(), from.y() + 1)));
        return MoveIf(soldier, current_dist,
                      QPoint(from.x() + 1, from.y() + 1), lag);
      }
    }
  };

  std::vector<Command> commands({Command::MoveLeftDown, Command::MoveLeftUp,
                                 Command::MoveRightUp, Command::MoveRightDown,
                                 Command::MoveLeft, Command::MoveRight,
                                 Command::MoveUp, Command::MoveDown});
  for (auto soldier : soldiers_) {
    if (soldier->GetSide() == Side::kDefender) continue;
    if (soldier->IsDead()) continue;
    soldier->MakeTick();
    if (soldier->GetTimeLag() > 0) continue;
    int distance = GetDistance(soldier->GetPosition());
    if (distance == 0) continue;
    RemoveSoldierFromCell(soldier);
    for (auto& command : commands) {
      IssueCommand(soldier, soldier->GetPosition(), distance, command);
    }
    PutSoldierToCell(soldier);
  }
}

void World::LoadMap(const QString& path, GameMode mode, Side side) {
  QFile file(path);

  if (!file.open(QIODevice::ReadOnly)) {
    qCritical("Cannot open file for reading");
  }
  QString text = file.readAll();
  QJsonDocument doc = QJsonDocument::fromJson(text.toUtf8());
  QJsonObject obj = doc.object();

  QJsonObject size = obj.value("Size").toObject();
  int height = size["Length"].toInt();
  int width = size["Width"].toInt();


  QJsonArray color_speed_values = obj["Colors and speed"].toArray();
  std::vector<std::pair<int64_t, int>> color_and_value;

  int64_t color;
  int speed;
  for (auto&& color_speed_value : color_speed_values) {
    color = color_speed_value.toObject()["Color"].toInteger();
    speed = color_speed_value.toObject()["Speed"].toInt();
    color_and_value.emplace_back(color, speed);
  }

  cells_.resize(height,
                std::vector<Cell>(width));

  QString map_string(obj["Map"].toString());
  QTextStream map_stream(&map_string);

  std::vector<std::vector<Landscape>>
      landscape_map(height, std::vector<Landscape>(width));

  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      int color_index;
      map_stream >> color_index;
      landscape_map[i][j] = Landscape(color_and_value[color_index].first,
                                      color_and_value[color_index].second);
      cells_[i][j] = Cell(color_and_value[color_index].first,
                          color_and_value[color_index].second);
    }
  }
  distances_map_ = std::move(GroundDistancesMap(std::move(landscape_map)));

  QJsonArray attackers = obj["Attackers"].toArray();
  QJsonArray defenders = obj["Defenders"].toArray();
  QJsonArray terrain_objects = obj["Terrain objects"].toArray();

  auto add_soldier = [buffer = &bot_soldier_buffer_](const QJsonArray& array) {
    int x, y;
    for (const auto& element : array) {
      x = element.toObject()["X"].toInt();
      y = element.toObject()["Y"].toInt();
      buffer->emplace_back(x, y);
    }
  };

  if (mode == GameMode::kBot) {
    if (side == Side::kDefender) {
      add_soldier(attackers);
    } else {
      add_soldier(defenders);
    }
  }

  for (auto&& terrain_object : terrain_objects) {
    int x = terrain_object.toObject()["X"].toInt();
    int y = terrain_object.toObject()["Y"].toInt();
    AddTower(QPoint(x, y));
  }

  file.close();
}

int World::GetDistance(const QPoint& position) {
  return distances_map_.GetDistance(position);
}

void World::PutSoldierToCell(const std::shared_ptr<Soldier>& soldier) {
  cells_[soldier->GetPosition().y()][soldier->GetPosition().x()].InsertSoldier(
      soldier);
}

void World::RemoveSoldierFromCell(const std::shared_ptr<Soldier>& soldier) {
  cells_[soldier->GetPosition().y()][soldier->GetPosition().x()].EraseSoldier(
      soldier);
}

void World::MoveBullets() {
  // TODO(AZYAVCHIKOV) temporary code
  int bullet_radius = 5;
  int repeat = 1;

  for (auto bullet = bullets_.begin(); bullet != bullets_.end();) {
    auto maybe_deleted_bullet_iterator = bullet;
    auto maybe_deleted_bullet = *bullet;
    ++bullet;
    for (int j = 0; j < repeat; ++j) {
      maybe_deleted_bullet->Move();
      DamageArea(maybe_deleted_bullet->GetPosition().x(),
                 maybe_deleted_bullet->GetPosition().y(),
                 bullet_radius,
                 maybe_deleted_bullet);
      if (maybe_deleted_bullet->IsUsed()) {
        bullets_.erase(maybe_deleted_bullet_iterator);
        break;
      }
    }
  }

  std::erase_if(soldiers_, [](const std::shared_ptr<Soldier>& soldier) {
    return soldier->IsDead();
  });
}

void World::DamageArea(int x, int y, int radius,
                       const std::shared_ptr<Bullet>& bullet) {
  QPoint top(0, 0);
  top.setX(std::max(top.x(), x - radius));
  top.setY(std::max(top.y(), y - radius));
  QPoint bottom(cells_[0].size() - 1, cells_.size() - 1);
  bottom.setX(std::min(bottom.x(), x + radius));
  bottom.setY(std::min(bottom.y(), y + radius));

  for (int i = top.y(); i <= bottom.y(); ++i) {
    for (int j = top.x(); j <= bottom.x(); ++j) {
      auto& container = cells_[i][j].GetSoldiers();
      for (auto soldier_iterator = container.begin();
           soldier_iterator != container.end();
           soldier_iterator++) {
        auto soldier = *soldier_iterator;
        if (soldier->GetSide() == bullet->GetSide()) continue;
        int damage = bullet->GetDamage();
        if (cells_[i][j].IsTrench()) {
          damage = static_cast<int>(damage * weapons::kTrenchEffect);
        }
        soldier->TakeDamage(damage);
        bullet->MakeUsed();
        if (soldier->IsDead()) {
          if (soldier->GetSide() == Side::kAttacker) {
            count_attackers_--;
          }
          cells_[i][j].InsertSoldier(soldier_iterator.operator*());
        }
        return;
      }
    }
  }
}

void World::MakeShots() {
  for (auto& soldier : soldiers_) {
    auto nearest = FindNearest(soldier);
    if (!nearest.has_value()) continue;
    auto bullet = soldier->Fire(soldier->GetPosition(),
                                nearest.value()->GetPosition());
    if (bullet.has_value()) {
      AddBullet(bullet.value());
    }
  }
}

std::optional<std::shared_ptr<Soldier>> World::FindNearest(
    const std::shared_ptr<Soldier>& current_soldier) const {
  std::optional<std::shared_ptr<Soldier>> nearest_soldier;
  int64_t dist = INT64_MAX, new_dist;
  QPoint to;
  QPoint from = current_soldier->GetPosition();

  for (auto& soldier : soldiers_) {
    if (current_soldier->GetSide() == soldier->GetSide()) continue;
    to = soldier->GetPosition();
    new_dist = (from.x() - to.x()) * (from.x() - to.x())
        + (from.y() - to.y()) * (from.y() - to.y());
    if (new_dist < dist) {
      dist = new_dist;
      nearest_soldier = soldier;
    }
  }

  return nearest_soldier;
}

void World::FireTower() {
  auto tower_iterator = towers_.begin();
  while (tower_iterator != towers_.end()) {
    auto& tower = *tower_iterator;
    Cell& cell =
        cells_[tower->GetPosition().y()][tower->GetPosition().x()];
    for (const auto& soldier : cell.GetSoldiers()) {
      tower->TakeDamage(soldier->GetTowerDamage());
    }
    auto maybe_deleted_tower = tower_iterator;
    ++tower_iterator;
    if ((*maybe_deleted_tower)->IsDead()) {
      distances_map_.EraseObject(*maybe_deleted_tower);
      towers_.erase(maybe_deleted_tower);
    }
  }
}

void World::UpdateCountAttackers() {
  count_attackers_ = 0;
  for (const auto& soldier : soldiers_) {
    if (soldier->GetSide() == Side::kAttacker) {
      count_attackers_++;
    }
  }
}

void World::LoadBotData(Side side) {
  if (!bot_soldier_buffer_.empty()) {
    for (const auto& point : bot_soldier_buffer_) {
      AddSoldier(side, point);
    }
  }
}

int World::GetCountAttackers() const {
  return count_attackers_;
}

int World::GetCountTowers() const {
  return towers_.size();
}
int World::GetLag(const QPoint& position) {
  return distances_map_.GetLag(position);
}

bool World::IsTrench(const QPoint& position) {
  return distances_map_.IsTrench(position);
}
void World::MakeTrench(const QPoint& position) {
  distances_map_.MakeTrench(position);
}
void World::RemoveTrench(const QPoint& position) {
  distances_map_.RemoveTrench(position);
}

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
  if (side == Side::kAttacker) {
    ++count_attackers_;
  }
  Cell& cell =
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

const QSize& World::GetSize() const {
  return distances_map_.GetSize();
}

void World::DrawObject(QPainter* painter, const QPoint& pos,
                       const QSize& size, const QPixmap& picture) {
  painter->save();
  int window_width = painter->window().width() - 1;
  int window_height = painter->window().height() - 1;
  QPoint screen_point;
  screen_point.setX((window_width * (2 * pos.x() + 1))
                        / (2 * GetSize().width()));
  screen_point.setY((window_height * (2 * pos.y() + 1))
                        / (2 * GetSize().height()));

  QPoint top_point = QPoint(screen_point.x() - size.width() / 2,
                            screen_point.y() - size.height() / 2);
  QPoint bottom_point = QPoint(screen_point.x() + size.width() / 2,
                               screen_point.y() + size.height() / 2);
  painter->drawPixmap(QRect(top_point, bottom_point),
                      picture);
  painter->restore();
}

QPixmap World::GetPixmap(const QSize& size, bool are_objects_visible) {
  QPixmap buffer(size);
  QPainter painter(&buffer);
  int window_width = painter.window().width() - 1;
  int window_height = painter.window().height() - 1;

  painter.drawPixmap(QRect(0, 0,
                           window_width + 1, window_height + 1),
                     distances_map_.GetPixmap());

  for (const auto& object : towers_) {
    DrawObject(&painter, object->GetPosition(),
               object->GetSize(), object->GetPixmap());
  }
  if (are_objects_visible) {
    for (const auto& soldier : soldiers_) {
      DrawObject(&painter, soldier->GetPosition(),
                 soldier->GetSize(), soldier->GetPixmap());
    }

    for (const auto& bullet : bullets_) {
      DrawObject(&painter, bullet->GetPosition(),
                 bullet->GetSize(), bullet->GetPixmap());
    }
  }
  return buffer;
}

void World::MoveSoldiers() {
  if (towers_.empty()) return;

  for (auto soldier : soldiers_) {
    if (soldier->GetSide() == Side::kDefender) continue;
    assert(!soldier->IsDead());
    RemoveSoldierFromCell(soldier);
    soldier->Move(distances_map_);
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
      cells_[i][j] = Cell();
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
        if (distances_map_.IsTrench(soldier->GetPosition())) {
          damage = static_cast<int>(damage * weapons::kTrenchEffect);
        }
        soldier->TakeDamage(damage);
        bullet->MakeUsed();
        if (soldier->IsDead()) {
          if (soldier->GetSide() == Side::kAttacker) {
            count_attackers_--;
          }
          cells_[i][j].EraseSoldier(soldier_iterator.operator*());
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
      if (soldier->GetSide() != Side::kDefender) {
        tower->TakeDamage(soldier->GetTowerDamage());
      }
    }
    auto maybe_deleted_tower = tower_iterator;
    ++tower_iterator;
    if ((*maybe_deleted_tower)->IsDead()) {
      distances_map_.EraseDeadObjects();
      towers_.erase(maybe_deleted_tower);
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

bool World::IsTrench(const QPoint& position) {
  return distances_map_.IsTrench(position);
}
void World::MakeTrench(const QPoint& position) {
  distances_map_.MakeTrench(position);
}
void World::RemoveTrench(const QPoint& position) {
  distances_map_.RemoveTrench(position);
}

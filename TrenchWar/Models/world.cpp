#include <iostream>
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
  picture_ = DrawWorld();
}

void World::AddSoldier(Side side) {
  QRect field({0, 0}, size_);
  AddSoldier(side, utils::RandomGenerator::GetRandomPoint(QRect(field)));
}

void World::AddSoldier(Side side, const QPoint& position) {
  assert(position.y() >= 0 && position.y() < cells_.size());
  assert(position.x() >= 0 && position.x() < cells_[position.y()].size());
  auto new_object = std::make_shared<Soldier>(side, position);
  auto& cell =
      cells_[new_object->GetPosition().y()][new_object->GetPosition().x()];
  soldiers_.push_back(new_object);
  cell.soldiers.insert(new_object);
}

void World::AddTower() {
  QRect field({0, 0}, size_);
  AddTower(utils::RandomGenerator::GetRandomPoint(QRect(field)));
}

void World::AddTower(const QPoint& position) {
  auto new_object = std::make_shared<Tower>(position);
  towers_.push_back(new_object);
  distance_loading_threads_.emplace(&World::GenerateNewDistances, this,
                                    std::ref(new_object->GetPosition()));
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

const std::vector<std::shared_ptr<Soldier>>& World::GetSoldiers() const {
  return soldiers_;
}

const std::vector<std::shared_ptr<Tower>>&
World::GetTowers() const {
  return towers_;
}

const std::vector<std::shared_ptr<Bullet>>& World::GetBullets() const {
  return bullets_;
}

const QSize& World::GetSize() const {
  return size_;
}

const World::Cell& World::GetCell(const QPoint& point) const {
  assert(point.y() >= 0 && point.y() < cells_.size());
  assert(point.x() >= 0 && point.x() < cells_[point.y()].size());
  return cells_[point.y()][point.x()];
}

World::Cell& World::GetCell(const QPoint& point) {
  assert(point.y() >= 0 && point.y() < cells_.size());
  assert(point.x() >= 0 && point.x() < cells_[point.y()].size());
  return cells_[point.y()][point.x()];
}

const QPixmap& World::GetPixmap() {
  return picture_;
}

void World::Update() {
  if (dead_soldiers_ * 2 > soldiers_.size()) {
    std::sort(soldiers_.begin(), soldiers_.end(),
              [&](std::shared_ptr<Soldier> soldier1,
                  std::shared_ptr<Soldier> soldier2) {
                return soldier1->GetHitPoints() > soldier2->GetHitPoints();
              });
    soldiers_.resize(soldiers_.size() - dead_soldiers_);
    dead_soldiers_ = 0;
  }
}

void World::MoveSoldiers() {
  while (!distance_loading_threads_.empty()) {
    distance_loading_threads_.front().join();
    distance_loading_threads_.pop();
  }

  if (towers_.empty()) return;

  auto Lag = [&](int x, int y) {
    return cells_[y][x].landscape.move_lag;
  };

  auto Distance = [&](int x, int y) {
    int result = INT32_MAX;
    for (int i = 0; i < distances_.size(); ++i) {
      result = std::min(result, distances_[i][y][x]);
    }
    return result;
  };

  auto MoveIf = [&](int soldier_index, int& current_dist,
                    int to_x, int to_y, int lag = 0) {
    int new_dist = Distance(to_x, to_y) + lag;
    if (current_dist > new_dist) {
      current_dist = new_dist;
      soldiers_[soldier_index]->SetPosition(QPoint(to_x, to_y));
      soldiers_[soldier_index]->SetTimeLag(Lag(to_x, to_y) + lag);
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

  auto IssueCommand = [&](int soldier_index,
                          int from_x, int from_y,
                          int& current_dist, Command command) {
    switch (command) {
      case Command::MoveLeft: {
        if (from_x == 0) return;

        return MoveIf(soldier_index, current_dist, from_x - 1, from_y);
      }
      case Command::MoveRight: {
        if (from_x == cells_[from_y].size() - 1) return;

        return MoveIf(soldier_index, current_dist, from_x + 1, from_y);
      }
      case Command::MoveUp: {
        if (from_y == 0) return;

        return MoveIf(soldier_index, current_dist, from_x, from_y - 1);
      }
      case Command::MoveDown: {
        if (from_y == cells_.size() - 1) return;

        return MoveIf(soldier_index, current_dist, from_x, from_y + 1);
      }
      case Command::MoveLeftDown: {
        if (from_x == 0 || from_y == cells_.size() - 1) return;

        int lag = std::min(Lag(from_x - 1, from_y),
                           Lag(from_x, from_y + 1));
        return MoveIf(soldier_index, current_dist,
                      from_x - 1, from_y + 1, lag);
      }
      case Command::MoveLeftUp: {
        if (from_x == 0 || from_y == 0) return;

        int lag = std::min(Lag(from_x - 1, from_y),
                           Lag(from_x, from_y - 1));
        return MoveIf(soldier_index, current_dist,
                      from_x - 1, from_y - 1, lag);
      }
      case Command::MoveRightUp: {
        if (from_x == cells_[from_y].size() - 1 || from_y == 0) return;

        int lag = std::min(Lag(from_x + 1, from_y),
                           Lag(from_x, from_y - 1));
        return MoveIf(soldier_index, current_dist,
                      from_x + 1, from_y - 1, lag);
      }
      case Command::MoveRightDown: {
        if (from_x == cells_[from_y].size() - 1
            || from_y == cells_.size() - 1)
          return;

        int lag = std::min(Lag(from_x + 1, from_y),
                           Lag(from_x, from_y + 1));
        return MoveIf(soldier_index, current_dist,
                      from_x + 1, from_y + 1, lag);
      }
    }
  };

  std::vector<Command> commands({Command::MoveLeftDown, Command::MoveLeftUp,
                                 Command::MoveRightUp, Command::MoveRightDown,
                                 Command::MoveLeft, Command::MoveRight,
                                 Command::MoveUp, Command::MoveDown});
  for (int i = 0; i < soldiers_.size(); ++i) {
    if (soldiers_[i]->GetSide() == Side::kDefender) continue;
    if (soldiers_[i]->IsDead()) continue;
    soldiers_[i]->MakeTick();
    if (soldiers_[i]->GetTimeLag() > 0) continue;
    int x = soldiers_[i]->GetPosition().x();
    int y = soldiers_[i]->GetPosition().y();
    int distance = Distance(x, y);
    if (distance == 0) continue;
    cells_[y][x].soldiers.erase(soldiers_[i]);
    for (int j = 0; j < commands.size(); ++j) {
      IssueCommand(i, x, y, distance, commands[j]);
    }
    x = soldiers_[i]->GetPosition().x();
    y = soldiers_[i]->GetPosition().y();
    cells_[y][x].soldiers.insert(soldiers_[i]);
  }
}

void World::LoadMap(const QString& path, GameMode mode, Side side) {
  QFile file(path);

  if (!file.open(QIODevice::ReadOnly)) {
    qCritical("Cannot open file for reading");
  }
  QString text = file.readAll();
  QJsonDocument doc = QJsonDocument::fromJson(text.toUtf8());
  QJsonObject obj  = doc.object();

  QJsonObject j_size  = obj.value("Size").toObject();
  int height, width;
  height = j_size["Length"].toInt();
  width = j_size["Width"].toInt();
  size_ = QSize(height, width);

  QJsonArray j_color_value = obj["Colors and speed"].toArray();
  std::vector<std::pair<int64_t, int>> color_and_value;

  int64_t color;
  int speed;
  for (int i = 0; i < j_color_value.size(); i++) {
    color = j_color_value.at(i).toObject()["Color"].toInteger();
    speed = j_color_value.at(i).toObject()["Speed"].toInt();
    color_and_value.emplace_back(color, speed);
  }


  cells_.resize(height,
                std::vector<Cell>(width));

  std::stringstream st(obj["Map"].toString().toStdString());
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      int color_index;
      st >> color_index;
      cells_[i][j].landscape = Landscape(color_and_value[color_index].first,
                                         color_and_value[color_index].second);
    }
  }

  QJsonArray attackers = obj["Attackers"].toArray();
  QJsonArray defenders = obj["Defenders"].toArray();
  QJsonArray terrain_objects = obj["Terrain objects"].toArray();

  auto add_soldier = [&](const QJsonArray& array) {
    int x, y;
    for (const auto & i : array) {
      x = i.toObject()["X"].toInt();
      y = i.toObject()["Y"].toInt();
      bot_soldier_buffer_.emplace_back(x, y);
    }
  };

  if (mode == GameMode::kBot) {
    int x, y;
    if (side == Side::kDefender) {
      add_soldier(attackers);
    } else {
      add_soldier(defenders);
    }
  }

  int x, y;
  for (auto && terrain_object : terrain_objects) {
    x = terrain_object.toObject()["X"].toInt();
    y = terrain_object.toObject()["Y"].toInt();
    AddTower(QPoint(x, y));
  }

  file.close();
}

QPixmap World::DrawWorld() const {
  QPixmap picture(image_sizes::kWorldImage);
  auto painter = QPainter(&picture);
  painter.save();
  int window_width = painter.window().width() - 1;
  int window_height = painter.window().height() - 1;
  for (int y = 0; y < cells_.size(); ++y) {
    for (int x = 0; x < cells_[y].size(); ++x) {
      int x_top = (window_width * x) / cells_[y].size();
      int x_bottom = ((window_width * (x + 1)) / cells_[y].size());
      int y_top = (window_height * y) / cells_.size();
      int y_bottom = ((window_height * (y + 1)) / cells_.size());
      QRect cell_rect(QPoint(x_top, y_top),
                      QPoint(x_bottom, y_bottom));
      QColor color = cells_[y][x].landscape.color;
      painter.setBrush(QBrush(color));
      painter.setPen(QPen(QColor(color), 1));
      painter.drawRect(cell_rect);
    }
  }
  painter.restore();
  return picture;
}

void World::GenerateNewDistances(const QPoint& pos) {
  std::lock_guard<std::mutex> lock(distances_mutex_);
  distances_.emplace_back(cells_.size(),
                          std::vector<int>(cells_[0].size(),
                                           std::numeric_limits<int>::max()));
  for (auto& cell_line : cells_) {
    for (auto& cell : cell_line) {
      cell.used = false;
    }
  }
  auto& container = distances_[distances_.size() - 1];

  auto cmp =
      [&](std::pair<int, int> left, std::pair<int, int> right) {
        return container[left.second][left.first]
            > container[right.second][right.first];
      };
  std::priority_queue<std::pair<int, int>,
                      std::vector<std::pair<int, int>>,
                      decltype(cmp)>
      latest_at_ground(cmp);
  container[pos.y()][pos.x()] = 0;
  latest_at_ground.push(std::make_pair(pos.x(), pos.y()));

  auto push_if =
      [&](int x, int y, int dist, bool condition = true) {
        if (!condition || cells_[y][x].used) {
          return;
        }
        if (container[y][x] > dist + cells_[y][x].landscape.move_lag) {
          container[y][x] = dist + cells_[y][x].landscape.move_lag;
          latest_at_ground.push(std::make_pair(x, y));
        }
      };

  while (!latest_at_ground.empty()) {
    int x = latest_at_ground.top().first;
    int y = latest_at_ground.top().second;
    int current_dist = container[y][x];

    // left neighbor
    push_if(x - 1, y, current_dist, (x != 0));
    // right neighbor
    push_if(x + 1, y, current_dist, (x != cells_[y].size() - 1));
    // upper neighbor
    push_if(x, y - 1, current_dist, (y != 0));
    // lower neighbor
    push_if(x, y + 1, current_dist, (y != cells_.size() - 1));

    cells_[y][x].used = true;
    latest_at_ground.pop();
  }
}

void World::MoveBullets() {
  // TODO(AZYAVCHIKOV) temporary code
  int bullet_radius = 5;
  int repeat = 1;

  for (int i = 0; i < bullets_.size(); ++i) {
    for (int j = 0; j < repeat; ++j) {
      assert(!bullets_[i]->IsUsed());
      bullets_[i]->Move();
      // DamageArea(bullets_[i]->GetPosition().x(),
      // bullets_[i]->GetPosition().y(),
      //            weapons::kBulletRadius, i);
      DamageArea(bullets_[i]->GetPosition().x(), bullets_[i]->GetPosition().y(),
                 bullet_radius, i);
      if (bullets_[i]->IsUsed()) {
        int last = bullets_.size() - 1;
        std::swap(bullets_[i], bullets_[last]);
        bullets_.erase(bullets_.begin() + last);
        --i;
        break;
      }
    }
  }
}

void World::DamageArea(int x, int y, int radius, int bullet_index) {
  QPoint top(0, 0);
  top.setX(std::max(top.x(), x - radius));
  top.setY(std::max(top.y(), y - radius));
  QPoint bottom(cells_[0].size() - 1, cells_.size() - 1);
  bottom.setX(std::min(bottom.x(), x + radius));
  bottom.setY(std::min(bottom.y(), y + radius));

  auto& bullet = bullets_[bullet_index];

  for (int i = top.y(); i <= bottom.y(); ++i) {
    for (int j = top.x(); j <= bottom.x(); ++j) {
      auto& container = cells_[i][j].soldiers;
      for (auto k = container.begin(); k != container.end(); k++) {
        if ((*k)->GetSide() == bullet->GetSide()) continue;
        int damage = bullet->GetDamage();
        if (cells_[i][j].is_trench) {
          damage = static_cast<int>(damage * weapons::kTrenchEffect);
        }
        (*k)->TakeDamage(damage);
        bullet->MakeUsed();
        if ((*k)->IsDead()) {
          if ((*k)->GetSide() == Side::kAttacker) {
            count_attackers_--;
          }
          cells_[i][j].soldiers.erase(k);
          ++dead_soldiers_;
        }
        return;
      }
    }
  }
}

void World::MakeShots() {
  for (int i = 0; i < soldiers_.size(); ++i) {
    if (soldiers_[i]->IsDead()) continue;
    auto nearest = FindNearest(soldiers_[i]);
    if (!nearest.has_value()) continue;
    auto bullet = soldiers_[i]->Fire(soldiers_[i]->GetPosition(),
                                     nearest.value()->GetPosition());
    if (bullet.has_value()) {
      AddBullet(bullet.value());
    }
  }
}

std::optional<std::shared_ptr<Soldier>> World::FindNearest(
    const std::shared_ptr<Soldier>& soldier) const {
  int nearest_index = -1;
  int64_t dist = INT64_MAX, new_dist;
  QPoint to;
  QPoint from = soldier->GetPosition();

  for (int i = 0; i < soldiers_.size(); ++i) {
    if (soldiers_[i]->IsDead()) continue;
    if (soldiers_[i]->GetSide() == soldier->GetSide()) continue;
    to = soldiers_[i]->GetPosition();
    new_dist = (from.x() - to.x()) * (from.x() - to.x())
        + (from.y() - to.y()) * (from.y() - to.y());
    if (new_dist < dist) {
      dist = new_dist;
      nearest_index = i;
    }
  }
  if (nearest_index == -1) return std::nullopt;

  return soldiers_[nearest_index];
}

void World::TrenchUpdate() {
  picture_ = DrawWorld();
}

void World::FireTower() {
  std::shared_ptr<Tower> temp = nullptr;
  for (int i = 0; i < towers_.size(); ++i) {
    auto& tower = towers_[i];
    Cell& cell =
        cells_[tower->GetPosition().y()][tower->GetPosition().x()];
    for (const auto& soldier : cell.soldiers) {
      tower->TakeDamage(soldier->GetTowerDamage());
    }
    if (tower->IsDestroyed()) {
      int last = towers_.size() - 1;
      std::swap(towers_[i], towers_[last]);
      towers_.erase(towers_.begin() + last);
      distances_[i] = std::move(distances_[last]);
      distances_.erase(distances_.begin() + last);
      --i;
      is_need_update_towers_ = true;
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

World::Landscape::Landscape(const QColor& q_color, int speed) {
  color = q_color;
  move_lag = speed;
}

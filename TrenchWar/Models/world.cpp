#include <iostream>
#include <random>
#include <utility>

#include "world.h"

#include "Models/Tools/settings.h"
#include <QAudioOutput>
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
  distances_.emplace_back(cells_.size(),
                          std::vector<int>(cells_[0].size(),
                                           std::numeric_limits<int>::max()));
  distance_loading_threads_.emplace(&World::GenerateNewDistances,
                                    this,
                                    --distances_.end(),
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
    for (auto& distance : distances_) {
      result = std::min(result, distance[y][x]);
    }
    return result;
  };

  auto MoveIf = [&](const std::shared_ptr<Soldier>& soldier, int& current_dist,
                    int to_x, int to_y, int lag = 0) {
    int new_dist = Distance(to_x, to_y) + lag;
    if (current_dist > new_dist) {
      current_dist = new_dist;
      soldier->SetPosition(QPoint(to_x, to_y));
      soldier->SetTimeLag(Lag(to_x, to_y) + lag);
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
                          int from_x, int from_y,
                          int& current_dist, Command command) {
    switch (command) {
      case Command::MoveLeft: {
        if (from_x == 0) return;

        return MoveIf(soldier, current_dist, from_x - 1, from_y);
      }
      case Command::MoveRight: {
        if (from_x == cells_[from_y].size() - 1) return;

        return MoveIf(soldier, current_dist, from_x + 1, from_y);
      }
      case Command::MoveUp: {
        if (from_y == 0) return;

        return MoveIf(soldier, current_dist, from_x, from_y - 1);
      }
      case Command::MoveDown: {
        if (from_y == cells_.size() - 1) return;

        return MoveIf(soldier, current_dist, from_x, from_y + 1);
      }
      case Command::MoveLeftDown: {
        if (from_x == 0 || from_y == cells_.size() - 1) return;

        int lag = std::min(Lag(from_x - 1, from_y),
                           Lag(from_x, from_y + 1));
        return MoveIf(soldier, current_dist,
                      from_x - 1, from_y + 1, lag);
      }
      case Command::MoveLeftUp: {
        if (from_x == 0 || from_y == 0) return;

        int lag = std::min(Lag(from_x - 1, from_y),
                           Lag(from_x, from_y - 1));
        return MoveIf(soldier, current_dist,
                      from_x - 1, from_y - 1, lag);
      }
      case Command::MoveRightUp: {
        if (from_x == cells_[from_y].size() - 1 || from_y == 0) return;

        int lag = std::min(Lag(from_x + 1, from_y),
                           Lag(from_x, from_y - 1));
        return MoveIf(soldier, current_dist,
                      from_x + 1, from_y - 1, lag);
      }
      case Command::MoveRightDown: {
        if (from_x == cells_[from_y].size() - 1
            || from_y == cells_.size() - 1)
          return;

        int lag = std::min(Lag(from_x + 1, from_y),
                           Lag(from_x, from_y + 1));
        return MoveIf(soldier, current_dist,
                      from_x + 1, from_y + 1, lag);
      }
    }
  };

  std::vector<Command> commands({Command::MoveLeftDown, Command::MoveLeftUp,
                                 Command::MoveRightUp, Command::MoveRightDown,
                                 Command::MoveLeft, Command::MoveRight,
                                 Command::MoveUp, Command::MoveDown});
  for (auto soldier = soldiers_.begin(); soldier != soldiers_.end();
       ++soldier) {
    if ((*soldier)->GetSide() == Side::kDefender) continue;
    (*soldier)->MakeTick();
    if ((*soldier)->GetTimeLag() > 0) continue;
    int x = (*soldier)->GetPosition().x();
    int y = (*soldier)->GetPosition().y();
    int distance = Distance(x, y);
    if (distance == 0) continue;
    cells_[y][x].soldiers.erase(*soldier);
    for (auto& command : commands) {
      IssueCommand((*soldier), x, y, distance, command);
    }
    x = (*soldier)->GetPosition().x();
    y = (*soldier)->GetPosition().y();
    cells_[y][x].soldiers.insert(*soldier);
  }
}

void World::LoadMap(const QString& path, GameMode mode, Side side) {
  QFile file(path);

  if (!file.open(QIODevice::ReadOnly)) {
    qCritical("Cannot open file for reading");
  }
  QTextStream in(&file);
  std::vector<std::pair<int64_t, int>> color_and_value;

  QString size = in.readLine();
  int size_int = size.toInt();
  for (int i = 0; i < size_int; i++) {
    int64_t color;
    int value;
    in >> color >> value;
    color_and_value.emplace_back(color, value);
  }
  in.readLine();

  int height, width;
  in >> height >> width;
  size_ = QSize(width, height);
  cells_.resize(height,
                std::vector<Cell>(width));
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      int color_index;
      in >> color_index;
      cells_[i][j].landscape = Landscape(color_and_value[color_index].first,
                                         color_and_value[color_index].second);
    }
  }
  in.readLine();
  in.readLine();

  size = in.readLine();
  size_int = size.toInt();

  for (int i = 0; i < size_int; i++) {
    int x;
    int y;
    QString type;
    in >> x >> y >> type;
    if (type == "kTerrainObject") {
      AddTower(QPoint(x, y));
    }
    if (mode == GameMode::kBot &&
        ((type == "kDefender" && side == Side::kAttacker) ||
        (type == "kAttacker" && side == Side::kDefender))) {
      bot_soldier_buffer_.emplace_back(x, y);
    }
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

void World::GenerateNewDistances(
    std::list<std::vector<std::vector<int>>>::iterator distances_map,
    const QPoint& pos) {
  std::lock_guard<std::mutex> lock(distances_mutex_);
  for (auto& cell_line : cells_) {
    for (auto& cell : cell_line) {
      cell.used = false;
    }
  }
  auto& container = *distances_map;

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

  for (auto bullet = bullets_.begin(); bullet != bullets_.end();) {
    auto bullet_for_possible_delete = bullet;
    ++bullet;
    for (int j = 0; j < repeat; ++j) {
      (*bullet_for_possible_delete)->Move();
      DamageArea((*bullet_for_possible_delete)->GetPosition().x(),
                 (*bullet_for_possible_delete)->GetPosition().y(),
                 bullet_radius,
                 (*bullet_for_possible_delete));
      if ((*bullet_for_possible_delete)->IsUsed()) {
        bullets_.erase(bullet_for_possible_delete);
        break;
      }
    }
  }

  // TODO (AZUAVCHIKOV) not best solution
  for (auto soldier = soldiers_.begin(); soldier != soldiers_.end();) {
    auto soldier_for_possible_delete = soldier;
    ++soldier;
    if ((*soldier_for_possible_delete)->IsDead()) {
      soldiers_.erase(soldier_for_possible_delete);
    }
  }
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
      auto& container = cells_[i][j].soldiers;
      for (auto soldier = container.begin(); soldier != container.end();
           soldier++) {
        if ((*soldier)->GetSide() == bullet->GetSide()) continue;
        int damage = bullet->GetDamage();
        if (cells_[i][j].is_trench) {
          damage = static_cast<int>(damage * weapons::kTrenchEffect);
        }
        (*soldier)->TakeDamage(damage);
        bullet->MakeUsed();
        if ((*soldier)->IsDead()) {
          if ((*soldier)->GetSide() == Side::kAttacker) {
            count_attackers_--;
          }
          cells_[i][j].soldiers.erase(soldier);
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

void World::TrenchUpdate() {
  picture_ = DrawWorld();
}

void World::FireTower() {
  auto tower = towers_.begin();
  auto distance = distances_.begin();
  for (; tower != towers_.end();) {
    Cell& cell =
        cells_[(*tower)->GetPosition().y()][(*tower)->GetPosition().x()];
    for (const auto& soldier : cell.soldiers) {
      (*tower)->TakeDamage(soldier->GetTowerDamage());
    }
    auto tower_for_possible_delete = tower;
    auto distance_for_possible_delete = distance;
    ++tower;
    ++distance;
    if ((*tower_for_possible_delete)->IsDestroyed()) {
      towers_.erase(tower_for_possible_delete);
      distances_.erase(distance_for_possible_delete);
      // maybe unused
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

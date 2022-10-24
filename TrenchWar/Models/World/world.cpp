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
  cell.InsertSoldier(new_object);
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
                                    distances_.size() - 1,
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

const Cell& World::GetCell(const QPoint& point) const {
  assert(point.y() >= 0 && point.y() < cells_.size());
  assert(point.x() >= 0 && point.x() < cells_[point.y()].size());
  return cells_[point.y()][point.x()];
}

Cell& World::GetCell(const QPoint& point) {
  assert(point.y() >= 0 && point.y() < cells_.size());
  assert(point.x() >= 0 && point.x() < cells_[point.y()].size());
  return cells_[point.y()][point.x()];
}

const QPixmap& World::GetPixmap() {
  return picture_;
}

void World::MoveSoldiers() {
  FinishLoadingMap();

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

  size_ = QSize(height, width);

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

  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      int color_index;
      map_stream >> color_index;
      cells_[i][j] = Cell(color_and_value[color_index].first,
                          color_and_value[color_index].second);
    }
  }

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

void World::FinishLoadingMap() {
  while (!distance_loading_threads_.empty()) {
    distance_loading_threads_.front().join();
    distance_loading_threads_.pop();
  }
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
      QColor color = cells_[y][x].GetColor();
      painter.setBrush(QBrush(color));
      painter.setPen(QPen(QColor(color), 1));
      painter.drawRect(cell_rect);
    }
  }
  painter.restore();
  return picture;
}

void World::GenerateNewDistances(int distances_map_index,
                                 const QPoint& pos) {
  std::lock_guard<std::mutex> lock(distances_mutex_);
  std::vector<std::vector<bool>>
      used_cells(cells_.size(), std::vector<bool>(cells_[0].size(), false));

  auto distances_map_iterator = distances_.begin();
  for (int index = 0; index < distances_map_index; ++index) {
    ++distances_map_iterator;
  }
  auto& distances_map = *distances_map_iterator;

  auto cmp =
      [&](std::pair<int, int> left, std::pair<int, int> right) {
        return distances_map[left.second][left.first]
            > distances_map[right.second][right.first];
      };
  std::priority_queue<std::pair<int, int>,
                      std::vector<std::pair<int, int>>,
                      decltype(cmp)>
      latest_at_ground(cmp);
  distances_map[pos.y()][pos.x()] = 0;
  latest_at_ground.push(std::make_pair(pos.x(), pos.y()));

  auto push_if =
      [&](int x, int y, int dist, bool condition = true) {
        if (!condition || used_cells[y][x]) {
          return;
        }
        if (distances_map[y][x] > dist + cells_[y][x].GetTimeLag()) {
          distances_map[y][x] = dist + cells_[y][x].GetTimeLag();
          latest_at_ground.push(std::make_pair(x, y));
        }
      };

  while (!latest_at_ground.empty()) {
    int x = latest_at_ground.top().first;
    int y = latest_at_ground.top().second;
    int current_dist = distances_map[y][x];

    // left neighbor
    push_if(x - 1, y, current_dist, (x != 0));
    // right neighbor
    push_if(x + 1, y, current_dist, (x != cells_[y].size() - 1));
    // upper neighbor
    push_if(x, y - 1, current_dist, (y != 0));
    // lower neighbor
    push_if(x, y + 1, current_dist, (y != cells_.size() - 1));

    used_cells[y][x] = true;
    latest_at_ground.pop();
  }
}

int World::GetDistance(const QPoint& position) {
  int result = std::numeric_limits<int>::max();
  for (auto& distance : distances_) {
    result = std::min(result, distance[position.y()][position.x()]);
  }
  return result;
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

  // TODO(AZUAVCHIKOV) not best solution
  for (auto soldier = soldiers_.begin(); soldier != soldiers_.end();) {
    auto maybe_deleted_soldier = soldier;
    ++soldier;
    if ((*maybe_deleted_soldier)->IsDead()) {
      soldiers_.erase(maybe_deleted_soldier);
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

void World::TrenchUpdate() {
  picture_ = DrawWorld();
}

void World::FireTower() {
  auto tower_iterator = towers_.begin();
  auto distance_iterator = distances_.begin();
  while (tower_iterator != towers_.end()) {
    auto& tower = *tower_iterator;
    Cell& cell =
        cells_[tower->GetPosition().y()][tower->GetPosition().x()];
    for (const auto& soldier : cell.GetSoldiers()) {
      tower->TakeDamage(soldier->GetTowerDamage());
    }
    auto maybe_deleted_tower = tower_iterator;
    auto maybe_deleted_distance = distance_iterator;
    ++tower_iterator;
    ++distance_iterator;
    if ((*maybe_deleted_tower)->IsDestroyed()) {
      towers_.erase(maybe_deleted_tower);
      distances_.erase(maybe_deleted_distance);
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
  return cells_[position.y()][position.x()].GetTimeLag();
}

World::~World() {
  FinishLoadingMap();
}

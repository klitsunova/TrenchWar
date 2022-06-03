#include <random>
#include <utility>

#include "world.h"

World::World(const QString& path) {
  LoadMap(path);
  picture_ = DrawWorld();
}

void World::AddSoldier(Side side) {
  auto new_object = std::make_shared<Soldier>(side);
  new_object->SetRandomPosition(size_);
  auto& cell =
      cells_[new_object->GetPosition().y()][new_object->GetPosition().x()];
  soldiers_.push_back(new_object);
  cell.soldiers.insert(new_object);
}

void World::AddSoldier(const QPoint& position, Side side) {
  assert(position.y() >= 0 && position.y() < cells_.size());
  assert(position.x() >= 0 && position.x() < cells_[position.y()].size());
  auto new_object = std::make_shared<Soldier>(position, side);
  auto& cell =
      cells_[new_object->GetPosition().y()][new_object->GetPosition().x()];
  soldiers_.push_back(new_object);
  cell.soldiers.insert(new_object);
}

void World::AddTower() {
  auto new_object = std::make_shared<Tower>();
  new_object->SetRandomPosition(size_);
  towers_.push_back(new_object);
}

void World::AddTower(const QPoint& position) {
  auto new_object = std::make_shared<Tower>();
  new_object->SetPosition(position);
  towers_.push_back(new_object);
}

void World::AddBullet(const std::shared_ptr<Bullet>& bullet) {
  assert(bullet.get() != nullptr);
  bullets_.push_back(bullet);
  emit Shot();
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
  if (is_need_update_towers_) {
    UpdateGroundDistances();
  }
  is_need_update_towers_ = false;
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
  Update();

  auto Lag = [&](int x, int y) {
    return cells_[y][x].landscape.move_lag;
  };

  auto Distance = [&](int x, int y) {
    return cells_[y][x].ground_distance;
  };

  auto MoveIf = [&](int soldier_index, int& current_dist,
                    int to_x, int to_y, int lag = 0) {
    if (current_dist > Distance(to_x, to_y) + lag) {
      current_dist = Distance(to_x, to_y) + lag;
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
  // // TODO(AZYAVCHIKOV) - maybe not best solution
  // std::shuffle(commands.begin(), commands.end(),
  //              std::mt19937(std::random_device()()));
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
    // std::shuffle(commands.begin() + 4, commands.end(),
    //              std::mt19937(std::random_device()()));
    for (int j = 0; j < commands.size(); ++j) {
      IssueCommand(i, x, y, distance, commands[j]);
    }
    x = soldiers_[i]->GetPosition().x();
    y = soldiers_[i]->GetPosition().y();
    cells_[y][x].soldiers.insert(soldiers_[i]);
  }
}

void World::LoadMap(const QString& path) {
  QFile file(path);

  if (!file.open(QIODevice::ReadOnly)) {
    qCritical("Cannot open file for reading");
  }
  QTextStream in(&file);
  std::vector<std::pair<int64_t, int>> color_and_value;

  QString size = in.readLine();
  int size_t = size.toInt();
  for (int i = 0; i < size_t; i++) {
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

void World::UpdateGroundDistances() {
  for (int i = 0; i < cells_.size(); ++i) {
    for (int j = 0; j < cells_[i].size(); ++j) {
      cells_[i][j].used = false;
      cells_[i][j].ground_distance = INT32_MAX;
    }
  }

  auto cmp =
      [&](std::pair<int, int> left, std::pair<int, int> right) {
        return cells_[left.second][left.first].ground_distance
            > cells_[right.second][right.first].ground_distance;
      };
  std::priority_queue<std::pair<int, int>,
                      std::vector<std::pair<int, int>>,
                      decltype(cmp)>
      latest_at_ground(cmp);

  for (auto& object : towers_) {
    int x = object->GetPosition().x();
    int y = object->GetPosition().y();
    cells_[y][x].ground_distance = 0;
    latest_at_ground.push(std::make_pair(x, y));
  }

  auto push_if =
      [&](int x, int y, int dist, bool condition = true) {
        if (!condition || cells_[y][x].used) {
          return;
        }
        if (cells_[y][x].ground_distance > dist
                + cells_[y][x].landscape.move_lag) {
          cells_[y][x].ground_distance = dist + cells_[y][x].landscape.move_lag;
          latest_at_ground.push(std::make_pair(x, y));
        }
      };

  while (!latest_at_ground.empty()) {
    int x = latest_at_ground.top().first;
    int y = latest_at_ground.top().second;
    int current_dist = cells_[y][x].ground_distance;

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
    new_dist = (from.x() - to.x()) * (from.x() - to.x()) +
        (from.y() - to.y()) * (from.y() - to.y());
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
    Cell& cell = cells_[tower->GetPosition().y()][tower->GetPosition().x()];
    for (const auto& soldier : cell.soldiers) {
      tower->TakeDamage(soldier->GetTowerDamage());
    }
    if (tower->IsDestroyed()) {
      int last = towers_.size() - 1;
      std::swap(towers_[i], towers_[last]);
      towers_.erase(towers_.begin() + last);
      --i;
      is_need_update_towers_ = true;
    }
  }
}

World::Landscape::Landscape(const QColor& q_color, int speed) {
  color = q_color;
  move_lag = speed;
}

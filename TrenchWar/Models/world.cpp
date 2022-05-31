#include <random>
#include <utility>

#include "world.h"

World::World(const QString& path) {
  LoadMap(path);
  picture_ = DrawWorld();
  AddTerrainObject();
}

void World::AddSoldier(Side side) {
  auto new_object = std::make_shared<Soldier>(side);
  new_object->SetRandomPosition(size_);
  auto& cell =
      cells_[new_object->GetPosition().y()][new_object->GetPosition().x()];
  (side == Side::kDefender)
      ? defenders_.push_back(new_object)
      : attackers_.push_back(new_object);
  cell.soldiers.insert(new_object);
}

void World::AddSoldier(const QPoint& position, Side side) {
  assert(position.y() >= 0 && position.y() < cells_.size());
  assert(position.x() >= 0 && position.x() < cells_[position.y()].size());
  auto new_object = std::make_shared<Soldier>(position, side);
  auto& cell =
      cells_[new_object->GetPosition().y()][new_object->GetPosition().x()];
  (side == Side::kDefender)
      ? defenders_.push_back(new_object)
      : attackers_.push_back(new_object);
  cell.soldiers.insert(new_object);
}

void World::AddTerrainObject() {
  auto new_object = std::make_shared<TerrainObject>();
  new_object->SetRandomPosition(size_);
  QPoint pos = new_object->GetPosition();
  auto& cell = cells_[pos.y()][pos.x()];
  cell.terrain_objects.push_back(new_object);
  terrain_objects_.push_back(new_object);
}

void World::AddBullet(const std::shared_ptr<Bullet>& bullet) {
  assert(bullet.get() != nullptr);
  bullets_.push_back(bullet);
}

const std::vector<std::shared_ptr<Soldier>>& World::GetDefenders() const {
  return defenders_;
}

const std::vector<std::shared_ptr<Soldier>>& World::GetAttackers() const {
  return attackers_;
}

const std::vector<std::shared_ptr<TerrainObject>>&
World::GetTerrainObjects() const {
  return terrain_objects_;
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
      attackers_[soldier_index]->SetPosition(QPoint(to_x, to_y));
      attackers_[soldier_index]->SetTimeLag(Lag(to_x, to_y) + lag);
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
  for (int i = 0; i < attackers_.size(); ++i) {
    if (attackers_[i]->IsDead()) continue;
    attackers_[i]->MakeTick();
    if (attackers_[i]->GetTimeLag() > 0) continue;
    int x = attackers_[i]->GetPosition().x();
    int y = attackers_[i]->GetPosition().y();
    int distance = Distance(x, y);
    if (distance == 0) continue;
    cells_[y][x].soldiers.erase(attackers_[i]);
    // std::shuffle(commands.begin() + 4, commands.end(),
    //              std::mt19937(std::random_device()()));
    for (int j = 0; j < commands.size(); ++j) {
      IssueCommand(i, x, y, distance, commands[j]);
    }
    x = attackers_[i]->GetPosition().x();
    y = attackers_[i]->GetPosition().y();
    cells_[y][x].soldiers.insert(attackers_[i]);
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

  for (auto& object: terrain_objects_) {
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
  int bullet_radius = 0;
  // int bullet_radius = 3;
  // int repeat = 4;
  int repeat = 1;

  for (int i = 0; i < bullets_.size(); ++i) {
    for (int j = 0; j < repeat; ++j) {
      if (bullets_[i]->IsUsed()) continue;
      bullets_[i]->Move();
      DamageArea(bullets_[i]->GetPosition().x(), bullets_[i]->GetPosition().y(),
                 bullet_radius, i);
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
        (*k)->TakeDamage(bullet->GetDamage());
        bullet->MakeUsed();
        if ((*k)->IsDead()) {
          cells_[i][j].soldiers.erase(k);
        }
        return;
      }
    }
  }
}

void World::MakeShots() {
  for (int i = 0; i < defenders_.size(); ++i) {
    if (defenders_[i]->IsDead()) continue;
    auto nearest = FindNearest(defenders_[i]);
    if (!nearest.has_value()) continue;
    auto bullet = defenders_[i]->Fire(defenders_[i]->GetPosition(),
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
  QPoint to, from;

  for (int i = 0; i < attackers_.size(); ++i) {
    if (attackers_[i]->IsDead()) continue;
    to = attackers_[i]->GetPosition();
    new_dist = (from.x() - to.x()) * (from.x() - to.x()) + (from.y() - to.y()) * (from.y() - to.y());
    if (new_dist < dist) {
      dist = new_dist;
      nearest_index = i;
    }
  }
  if (nearest_index == -1) return std::nullopt;

  return attackers_[nearest_index];
}

void World::TrenchUpdate() {
  picture_ = DrawWorld();
}

World::Landscape::Landscape(const QColor& q_color, int speed) {
  color = q_color;
  move_lag = speed;
}

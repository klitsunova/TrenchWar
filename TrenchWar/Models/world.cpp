#include <random>
#include <utility>

#include "world.h"

World::World(const QString& path) {
  LoadMap(path);
  picture_ = DrawWorld();
  AddTerrainObject();
}

void World::AddSoldier(Soldier::Type type) {
  auto new_object = std::make_shared<Soldier>(type);
  new_object->SetRandomPosition(size_);
  if (type == Soldier::Type::kAttacker) {
    attackers_.push_back(new_object);
    cells_[new_object->GetPosition().y()][new_object->GetPosition().x()].attackers.insert(
        new_object);
  } else if (type == Soldier::Type::kDefender) {
    defenders_.push_back(new_object);
    cells_[new_object->GetPosition().y()][new_object->GetPosition().x()].defenders.insert(
        new_object);
  }
}

void World::AddSoldier(const QPoint& position, Soldier::Type type) {
  assert(position.y() >= 0 && position.y() < cells_.size());
  assert(position.x() >= 0 && position.x() < cells_[position.y()].size());
  auto new_object = std::make_shared<Soldier>(position, type);
  if (type == Soldier::Type::kAttacker) {
    attackers_.push_back(new_object);
    cells_[new_object->GetPosition().y()][new_object->GetPosition().x()].attackers.insert(
        new_object);
  } else if (type == Soldier::Type::kDefender) {
    defenders_.push_back(new_object);
    cells_[new_object->GetPosition().y()][new_object->GetPosition().x()].defenders.insert(
        new_object);
  }

}

void World::AddTerrainObject() {
  auto new_object = std::make_shared<TerrainObject>();
  new_object->SetRandomPosition(size_);
  QPoint pos = new_object->GetPosition();
  cells_[pos.y()][pos.x()].terrain_objects_.push_back(new_object);
  terrain_objects_.push_back(new_object);
}

void World::AddBullet(const QPoint& from, const QPoint& to,
                      Soldier::Type type, int damage) {
  assert(to.y() >= 0 && to.y() < cells_.size());
  assert(to.x() >= 0 && to.x() < cells_[to.y()].size());
  assert(from.y() >= 0 && from.y() < cells_.size());
  assert(from.x() >= 0 && from.x() < cells_[from.y()].size());
  std::shared_ptr<Bullet> new_bullet =
      std::make_shared<Bullet>(from, to, type, damage);
  bullets_.push_back(new_bullet);
}

const std::vector<std::shared_ptr<Soldier>>& World::GetDefenders() const {
  return defenders_;
}

const std::vector<std::shared_ptr<Soldier>>& World::GetAttackers() const {
  return attackers_;
}

const std::vector<std::shared_ptr<TerrainObject>>& World::GetTerrainObjects() const {
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

const QPixmap& World::GetPixmap() const {
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
    // assert(command_index >= 0 && command_index <= 7);
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
                                 Command::MoveUp, Command::MoveDown,});
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
    cells_[y][x].attackers.erase(attackers_[i]);
    // std::shuffle(commands.begin() + 4, commands.end(),
    //              std::mt19937(std::random_device()()));
    for (int j = 0; j < commands.size(); ++j) {
      IssueCommand(i, x, y, distance, commands[j]);
    }
    x = attackers_[i]->GetPosition().x();
    y = attackers_[i]->GetPosition().y();
    cells_[y][x].attackers.insert(attackers_[i]);
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
  int damage = bullets_[bullet_index]->GetDamage();

  for (int i = top.y(); i <= bottom.y(); ++i) {
    for (int j = top.x(); j <= bottom.x(); ++j) {
      if (bullets_[bullet_index]->GetType() == Soldier::Type::kDefender) {
        if (cells_[i][j].defenders.empty()) continue;

        (*cells_[i][j].defenders.begin())->TakeDamage(damage);
        bullets_[bullet_index]->MakeUsed();
        if ((*cells_[i][j].defenders.begin())->IsDead()) {
          cells_[i][j].defenders.erase(cells_[i][j].defenders.begin());
        }
        return;
      } else if (bullets_[bullet_index]->GetType()
          == Soldier::Type::kAttacker) {
        if (cells_[i][j].attackers.empty()) continue;

        (*cells_[i][j].attackers.begin())->TakeDamage(damage);
        bullets_[bullet_index]->MakeUsed();
        if ((*cells_[i][j].attackers.begin())->IsDead()) {
          cells_[i][j].attackers.erase(cells_[i][j].attackers.begin());
        }
        return;
      }
    }
  }
}

void World::MakeShot(std::shared_ptr<Soldier>& soldier) {
  assert(!soldier->IsDead());
  int nearest_index = -1;
  int64_t from_x = soldier->GetPosition().x();
  int64_t from_y = soldier->GetPosition().y();
  int64_t dist = INT64_MAX, new_dist;
  int64_t to_x, to_y;

  if (soldier->GetType() == Soldier::Type::kDefender) {
    for (int i = 0; i < attackers_.size(); ++i) {
      if (attackers_[i]->IsDead()) continue;
      to_x = attackers_[i]->GetPosition().x();
      to_y = attackers_[i]->GetPosition().y();
      new_dist =
          (from_x - to_x) * (from_x - to_x) + (from_y - to_y) * (from_y - to_y);
      if (new_dist < dist) {
        dist = new_dist;
        nearest_index = i;
      }
    }
    if (nearest_index == -1) return;
    to_x = attackers_[nearest_index]->GetPosition().x();
    to_y = attackers_[nearest_index]->GetPosition().y();
    AddBullet(QPoint(from_x, from_y), QPoint(to_x, to_y),
              Soldier::Type::kAttacker);
  } else if (soldier->GetType() == Soldier::Type::kAttacker) {
    for (int i = 0; i < defenders_.size(); ++i) {
      if (defenders_[i]->IsDead()) continue;
      to_x = defenders_[i]->GetPosition().x();
      to_y = defenders_[i]->GetPosition().y();
      new_dist =
          (from_x - to_x) * (from_x - to_x) + (from_y - to_y) * (from_y - to_y);
      if (new_dist < dist) {
        dist = new_dist;
        nearest_index = i;
      }
    }
    if (nearest_index == -1) return;
    to_x = defenders_[nearest_index]->GetPosition().x();
    to_y = defenders_[nearest_index]->GetPosition().y();
    AddBullet(QPoint(from_x, from_y), QPoint(to_x, to_y),
              Soldier::Type::kDefender);
  }
}

void World::MakeShots() {
  // for (int i = 0; i < attackers_.size(); ++i) {
  //   if (attackers_[i]->IsDead()) continue;
  //   MakeShot(attackers_[i]);
  // }
  for (int i = 0; i < defenders_.size(); ++i) {
    if (defenders_[i]->IsDead()) continue;
    MakeShot(defenders_[i]);
  }
}

World::Landscape::Landscape(const QColor& q_color, int speed) {
  color = q_color;
  move_lag = speed;
}

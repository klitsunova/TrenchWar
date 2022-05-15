#include <random>
#include <utility>

#include "world.h"

World::World(const QString& path) {
  LoadMap(path);
  picture_ = DrawWorld();
}

// const std::vector<std::shared_ptr<Soldier>>& World::GetSoldiers() const {
//   return soldiers_;
// }
//
// std::vector<std::shared_ptr<Soldier>>& World::GetSoldiers() {
//   return soldiers_;
// }

void World::AddSoldier() {
  auto new_object = std::make_shared<Soldier>();
  new_object->SetRandomPosition(size_);
  // soldiers_.push_back(new_object);
  attackers_.push_back(new_object);
  game_objects_.push_back(new_object);
}

void World::AddSoldier(const QPoint& position, Soldier::Type type) {
  assert(position.y() >= 0 && position.y() < cells_.size());
  assert(position.x() >= 0 && position.x() < cells_[position.y()].size());
  auto new_object = std::make_shared<Soldier>(position, type);
  // soldiers_.push_back(new_object);
  if (type == Soldier::Type::kAttacker) {
    attackers_.push_back(new_object);
  } else if (type == Soldier::Type::kDefender) {
    defenders_.push_back(new_object);
  }
  game_objects_.push_back(new_object);
}

void World::AddTerrainObject() {
  auto new_object = std::make_shared<TerrainObject>();
  new_object->SetRandomPosition(size_);
  QPoint pos = new_object->GetPosition();
  cells_[pos.y()][pos.x()].terrain_objects.push_back(new_object);
  game_objects_.push_back(new_object);
}

const std::vector<std::shared_ptr<GameObject>>& World::GetGameObjects() const {
  return game_objects_;
}

std::vector<std::shared_ptr<GameObject>>& World::GetGameObjects() {
  return game_objects_;
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
  assert(point.x() >= 0 && point.x() < cells_.size());
  assert(point.y() >= 0 && point.y() < cells_[point.x()].size());
  return cells_[point.x()][point.y()];
}

const QPixmap& World::GetPixmap() const {
  return picture_;
}

#include <iostream>
void World::UpdateDistances() {
  if (is_need_update_defenders) {
    is_need_update_defenders = false;
    UpdateGroundDistances();
    UpdateAirDistances();
  }
}

void World::MoveSoldiers() {
  UpdateDistances();

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

  auto IssueCommand = [&](int soldier_index,
                          int from_x, int from_y,
                          int& current_dist, int command_index) {
    assert(command_index >= 0 && command_index <= 7);
    switch (command_index) {
      case 0: {
        // left neighbor
        if (from_x == 0) return;

        return MoveIf(soldier_index, current_dist, from_x - 1, from_y);
      }
      case 1: {
        // right neighbor
        if (from_x == cells_[from_y].size() - 1) return;

        return MoveIf(soldier_index, current_dist, from_x + 1, from_y);
      }
      case 2: {
        // upper neighbor
        if (from_y == 0) return;

        return MoveIf(soldier_index, current_dist, from_x, from_y - 1);
      }
      case 3: {
        // lower neighbor
        if (from_y == cells_.size() - 1) return;

        return MoveIf(soldier_index, current_dist, from_x, from_y + 1);
      }
      case 4: {
        // left lower neighbor
        if (from_x == 0 || from_y == cells_.size() - 1) return;

        int lag = std::min(Lag(from_x - 1, from_y),
                           Lag(from_x, from_y + 1));
        return MoveIf(soldier_index, current_dist,
                      from_x - 1, from_y + 1, lag);
      }
      case 5: {
        // left upper neighbor
        if (from_x == 0 || from_y == 0) return;

        int lag = std::min(Lag(from_x - 1, from_y),
                           Lag(from_x, from_y - 1));
        return MoveIf(soldier_index, current_dist,
                      from_x - 1, from_y - 1, lag);
      }
      case 6: {
        // right upper neighbor
        if (from_x == cells_[from_y].size() - 1 || from_y == 0) return;

        int lag = std::min(Lag(from_x + 1, from_y),
                           Lag(from_x, from_y - 1));
        return MoveIf(soldier_index, current_dist,
                      from_x + 1, from_y - 1, lag);
      }
      case 7: {
        // right lower neighbor
        if (from_x == cells_[from_y].size() - 1
            || from_y == cells_.size() - 1) return;

        int lag = std::min(Lag(from_x + 1, from_y),
                           Lag(from_x, from_y + 1));
        return MoveIf(soldier_index, current_dist,
                      from_x + 1, from_y + 1, lag);
      }
    }
  };

  std::vector<int> commands({4, 5, 6, 7, 0, 1, 2, 3});
  // // TODO(AZYAVCHIKOV) - maybe not best solution
  // std::shuffle(commands.begin(), commands.end(),
  //              std::mt19937(std::random_device()()));

  for (int i = 0; i < attackers_.size(); ++i) {
    attackers_[i]->MakeTick();
    if (attackers_[i]->GetTimeLag() > 0) continue;
    int x = attackers_[i]->GetPosition().x();
    int y = attackers_[i]->GetPosition().y();
    int distance = Distance(x, y);
    if (distance == 0) continue;
    // std::shuffle(commands.begin() + 4, commands.end(),
    //              std::mt19937(std::random_device()()));
    for (int j = 0; j < commands.size(); ++j) {
      IssueCommand(i, x, y, distance, commands[j]);
    }
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
  QPixmap picture(window_sizes::kWorldPicture);
  auto painter = QPainter(&picture);
  painter.save();
  painter.translate(QPoint(0, 0));
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

void World::UpdateAirDistances() {
  for (int i = 0; i < cells_.size(); ++i) {
    for (int j = 0; j < cells_[i].size(); ++j) {
      cells_[i][j].used = false;
      cells_[i][j].air_distance = INT32_MAX;
    }
  }

  std::queue<std::pair<int, int>> latest_at_air;

  for (auto& defender: defenders_) {
    int x = defender->GetPosition().x();
    int y = defender->GetPosition().y();
    cells_[y][x].used = true;
    cells_[y][x].air_distance = 0;
    latest_at_air.push(std::make_pair(x, y));
  }

  auto push_if =
      [&](int x, int y, int dist, bool condition = true) {
        if (!condition || cells_[y][x].used) {
          return;
        }
        if (cells_[y][x].air_distance > dist + 1) {
          cells_[y][x].air_distance = dist + cells_[y][x].landscape.move_lag;
          cells_[y][x].used = true;
          latest_at_air.push(std::make_pair(x, y));
        }
      };

  while (!latest_at_air.empty()) {
    int x = latest_at_air.front().first;
    int y = latest_at_air.front().second;
    int current_dist = cells_[y][x].air_distance;

    // left neighbor
    push_if(x - 1, y, current_dist, (x != 0));
    // right neighbor
    push_if(x + 1, y, current_dist, (x != cells_[y].size() - 1));
    // upper neighbor
    push_if(x, y - 1, current_dist, (y != 0));
    // lower neighbor
    push_if(x, y + 1, current_dist, (y != cells_.size() - 1));

    latest_at_air.pop();
  }
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

  for (auto& defender: defenders_) {
    int x = defender->GetPosition().x();
    int y = defender->GetPosition().y();
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

World::Landscape::Landscape(const QColor& q_color, int speed) {
  color = q_color;
  move_lag = speed;
}

#include "GroundMovingObject.h"

namespace {
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
}  // namespace

GroundMovingObject::GroundMovingObject(Side side, const QPoint& position)
    : MovingObject(side, position) {}

void GroundMovingObject::Move(const GroundDistancesMap& distances_map) {
  assert(!IsDead());

  int distance = distances_map.GetDistance(position_);
  if (distance == 0) return;

  MakeTick();
  if (GetTimeLag() > 0) return;

  QPoint start_position = position_;

  auto MoveIf = [this, &distances_map](int& current_dist,
                                       const QPoint& to, int lag = 0) {
    int new_dist = distances_map.GetDistance(to) + lag;
    if (current_dist > new_dist) {
      current_dist = new_dist;
      position_ = to;
      AddTimeLag(distances_map.GetLag(to) + lag);
    }
  };

  auto IssueCommand = [&distances_map, MoveIf](const QPoint& from,
                                               int& current_dist,
                                               Command command) {
    switch (command) {
      case Command::MoveLeft: {
        if (from.x() == 0) return;

        return MoveIf(current_dist, QPoint(from.x() - 1, from.y()));
      }
      case Command::MoveRight: {
        if (from.x() + 1 == distances_map.GetSize().width()) return;

        return MoveIf(current_dist, QPoint(from.x() + 1, from.y()));
      }
      case Command::MoveUp: {
        if (from.y() == 0) return;

        return MoveIf(current_dist, QPoint(from.x(), from.y() - 1));
      }
      case Command::MoveDown: {
        if (from.y() + 1 == distances_map.GetSize().height()) return;

        return MoveIf(current_dist, QPoint(from.x(), from.y() + 1));
      }
      case Command::MoveLeftDown: {
        if (from.x() == 0
            || from.y() + 1 == distances_map.GetSize().height())
          return;

        int lag =
            std::min(distances_map.GetLag(QPoint(from.x() - 1, from.y())),
                     distances_map.GetLag(QPoint(from.x(), from.y() + 1)));
        return MoveIf(current_dist, QPoint(from.x() - 1, from.y() + 1), lag);
      }
      case Command::MoveLeftUp: {
        if (from.x() == 0 || from.y() == 0) return;

        int lag =
            std::min(distances_map.GetLag(QPoint(from.x() - 1, from.y())),
                     distances_map.GetLag(QPoint(from.x(), from.y() - 1)));
        return MoveIf(current_dist, QPoint(from.x() - 1, from.y() - 1), lag);
      }
      case Command::MoveRightUp: {
        if (from.x() + 1 == distances_map.GetSize().width()
            || from.y() == 0) {
          return;
        }

        int lag =
            std::min(distances_map.GetLag(QPoint(from.x() + 1, from.y())),
                     distances_map.GetLag(QPoint(from.x(), from.y() - 1)));
        return MoveIf(current_dist, QPoint(from.x() + 1, from.y() - 1), lag);
      }
      case Command::MoveRightDown: {
        if (from.x() + 1 == distances_map.GetSize().width()
            || from.y() + 1 == distances_map.GetSize().height()) {
          return;
        }

        int lag =
            std::min(distances_map.GetLag(QPoint(from.x() + 1, from.y())),
                     distances_map.GetLag(QPoint(from.x(), from.y() + 1)));
        return MoveIf(current_dist, QPoint(from.x() + 1, from.y() + 1), lag);
      }
    }
  };

  std::vector<Command> commands({Command::MoveLeftDown, Command::MoveLeftUp,
                                 Command::MoveRightUp, Command::MoveRightDown,
                                 Command::MoveLeft, Command::MoveRight,
                                 Command::MoveUp, Command::MoveDown});

  for (auto& command : commands) {
    IssueCommand(start_position, distance, command);
  }
}

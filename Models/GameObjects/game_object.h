#ifndef MODELS_GAMEOBJECTS_GAME_OBJECT_H_
#define MODELS_GAMEOBJECTS_GAME_OBJECT_H_

#include <utility>
#include <QColor>

class GameObject {
  using Point = std::pair<int, int>;

 public:
  GameObject();
  explicit GameObject(const Point& position);
  ~GameObject() = default;

  Point GetPosition() const;
  void SetPosition(const Point& position);
  void SetRandomPosition();
  void MoveLeft();
  void MoveRight();
  void MoveUp();
  void MoveDown();
  virtual QColor GetColor();

 protected:
  Point position_;
  QColor color_ = Qt::white;
};

#endif  // MODELS_GAMEOBJECTS_GAME_OBJECT_H_

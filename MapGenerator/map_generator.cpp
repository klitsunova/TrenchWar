#include "map_generator.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

MapGenerator::MapGenerator(QWidget* parent)
    : QWidget(parent),
      original_picture_shell_(new ImageSHell(this)),
      changed_picture_shell_(new ImageSHell(this)),
      using_colors_shell_(new ImageSHell(this)),
      color_shell_(new ImageSHell(this)),
      draw_button_(new QPushButton("Draw", this)),
      change_button_(new QPushButton("Draw color changes", this)),
      restore_button_(new QPushButton("Restore color changes", this)),
      delete_objects_button_(new QPushButton("Delete soldiers", this)),
      save_button_(new QPushButton("Save changes", this)),
      load_button_(new QPushButton("Load new image", this)),
      add_color_button_(new QPushButton("<--", this)),
      cancel_color_button_(new QPushButton("Cancel", this)),
      add_soldier_button_(new QPushButton("Add", this)),
      cancel_soldier_button_(new QPushButton("Cancel", this)),
      width_(new QLineEdit("500", this)),
      height_(new QLineEdit("500", this)),
      speed_characteristic_(new QLineEdit("0", this)),
      speed_characteristic_label_(new QLabel("Speed characteristic",
                                             this)),
      object_variants_(new QComboBox(this)) {
  ManageLayout();

  CreateConnections();

  LoadImageFromFile("../map1.png");

  SetSoldiersMenuVisible(false);
}

void MapGenerator::ManageLayout() {
  auto layout1 = new QHBoxLayout();
  layout1->addWidget(original_picture_shell_, 10);
  layout1->addStretch(1);
  layout1->addWidget(changed_picture_shell_, 10);

  auto layout2 = new QVBoxLayout();
  layout2->addStretch(1);
  layout2->addLayout(layout1, 30);
  layout2->addStretch(1);

  auto layout3 = new QHBoxLayout();
  layout3->addWidget(using_colors_shell_, 5);
  auto layout4 = new QVBoxLayout();
  layout4->addStretch(1);
  layout4->addWidget(add_color_button_);
  layout4->addWidget(cancel_color_button_);
  layout4->addStretch(1);
  layout3->addLayout(layout4);
  layout3->addWidget(color_shell_, 2);

  auto layout5 = new QVBoxLayout();
  layout5->addStretch(1);
  layout5->addWidget(speed_characteristic_label_);
  layout5->addWidget(speed_characteristic_);
  layout5->addStretch(1);
  layout3->addLayout(layout5);

  auto layout6 = new QVBoxLayout();
  QStringList variants = {"Attacker", "Defender", "Tower"};
  object_variants_->addItems(variants);
  layout6->addWidget(object_variants_);
  layout6->addWidget(add_soldier_button_);
  layout6->addWidget(cancel_soldier_button_);
  layout3->addStretch(1);
  layout3->addLayout(layout6);

  layout2->addLayout(layout3, 3);
  layout2->addStretch(1);

  auto layout7 = new QVBoxLayout();
  layout7->addStretch(1);
  layout7->addWidget(load_button_);
  layout7->addStretch(1);
  auto label1 = new QLabel("Width", this);
  layout7->addWidget(label1, 0);
  layout7->addWidget(width_, 0);
  auto label2 = new QLabel("Height", this);
  layout7->addWidget(label2, 0);
  layout7->addWidget(height_, 0);
  layout7->addStretch(1);
  layout7->addWidget(draw_button_, 0);
  layout7->addWidget(change_button_, 0);
  layout7->addWidget(restore_button_, 0);
  layout7->addWidget(delete_objects_button_, 0);
  layout7->addWidget(save_button_, 0);
  layout7->addStretch(1);

  auto layout8 = new QHBoxLayout(this);
  layout8->addStretch(1);
  layout8->addLayout(layout2, 30);
  layout8->addStretch(1);
  layout8->addLayout(layout7, 0);
  layout8->addStretch(1);
}

void MapGenerator::CreateConnections() {
  connect(draw_button_,
          &QPushButton::clicked,
          this,
          &MapGenerator::DrawButtonClicked);
  connect(add_color_button_,
          &QPushButton::clicked,
          this,
          &MapGenerator::AddColorButtonClicked);
  connect(cancel_color_button_,
          &QPushButton::clicked,
          this,
          &MapGenerator::CancelColorButtonClicked);
  connect(add_soldier_button_,
          &QPushButton::clicked,
          this,
          &MapGenerator::AddSoldierButtonClicked);
  connect(cancel_soldier_button_,
          &QPushButton::clicked,
          this,
          &MapGenerator::CancelSoldierButtonClicked);
  connect(change_button_,
          &QPushButton::clicked,
          this,
          &MapGenerator::ChangeButtonClicked);
  connect(restore_button_,
          &QPushButton::clicked,
          this,
          &MapGenerator::RestoreButtonClicked);
  connect(delete_objects_button_,
          &QPushButton::clicked,
          this,
          &MapGenerator::DeleteObjectsButtonClicked);
  connect(save_button_,
          &QPushButton::clicked,
          this,
          &MapGenerator::SaveButtonClicked);
  connect(load_button_,
          &QPushButton::clicked,
          this,
          &MapGenerator::LoadButtonClicked);
}

void MapGenerator::ConvertImageToArray(const QSize& size) {
  assert(!buffer_picture_.isNull());

  if (map_.size() > 0 && size.height() == map_.size()
      && size.width() == map_[0].size()) {
    return;
  }

  using_original_colors = true;
  save_button_->setVisible(false);

  map_.resize(size.height());
  for (int i = 0; i < map_.size(); ++i) {
    map_[i].resize(size.width());
  }

  int window_width = buffer_picture_.width() - 1;
  int window_height = buffer_picture_.height() - 1;
  for (int i = 0; i < size.height(); ++i) {
    for (int j = 0; j < size.width(); ++j) {
      int x_top = (j * window_width) / size.width();
      int y_top = (i * window_height) / size.height();

      int x_bottom = ((j + 1) * window_width) / size.width();
      int y_bottom = ((i + 1) * window_height) / size.height();

      int points_number = 0;
      int64_t r = 0, g = 0, b = 0;

      for (int x = x_top; x <= x_bottom; ++x) {
        for (int y = y_top; y <= y_bottom; ++y) {
          ++points_number;
          r += qRed(buffer_picture_.pixel(x, y));
          g += qGreen(buffer_picture_.pixel(x, y));
          b += qBlue(buffer_picture_.pixel(x, y));
        }
      }
      map_[i][j].original_color = QColor(r / points_number,
                                         g / points_number,
                                         b / points_number).rgb();
      map_[i][j].using_color = -1;
    }
  }
}

void MapGenerator::DrawChangedPicture() {
  assert(!map_.empty());
  QPainter painter;
  painter.begin(&changed_picture_shell_->picture_);
  int map_width, map_height;
  int window_width = painter.window().width() - 1;
  int window_height = painter.window().height() - 1;
  map_height = map_.size();
  map_width = map_[0].size();
  QColor color;
  for (int i = 0; i < map_.size(); ++i) {
    for (int j = 0; j < map_[i].size(); ++j) {
      if (using_original_colors) {
        color = map_[i][j].original_color;
      } else {
        assert(map_[i][j].using_color >= 0
                   && map_[i][j].using_color < using_colors_.size());
        color = using_colors_[map_[i][j].using_color].color;
      }
      painter.setBrush(color);
      painter.setPen(QPen(color, 0));

      QPoint top = QPoint((j * window_width) / map_width,
                          (i * window_height) / map_height);
      QPoint
          bottom = QPoint(((j + 1) * window_width) / map_width,
                          ((i + 1) * window_height) / map_height);

      painter.drawRect(QRect(top, bottom));
    }
  }
  painter.end();
  DrawObjects();
  changed_picture_shell_->update();
}

void MapGenerator::DrawUsingColors() {
  assert(!using_colors_.empty());
  QPainter painter(&using_colors_shell_->picture_);
  int window_width = painter.window().width() - 1;
  int window_height = painter.window().height() - 1;
  for (int i = 0; i < using_colors_.size(); ++i) {
    painter.setBrush(using_colors_[i].color);
    painter.setPen(QPen(using_colors_[i].color, 0));

    QPoint top = QPoint((i * window_width) / using_colors_.size(),
                        0);
    QPoint bottom =
        QPoint(((i + 1) * window_width) / using_colors_.size(),
               window_height);

    painter.drawRect(QRect(top, bottom));
  }
  using_colors_shell_->update();
}

void MapGenerator::DrawObjects() {
  assert(!map_.empty());
  QPainter painter(&changed_picture_shell_->picture_);
  assert(painter.isActive());
  int map_width, map_height;
  int window_width = painter.window().width() - 1;
  int window_height = painter.window().height() - 1;
  map_height = map_.size();
  map_width = map_[0].size();
  for (int i = 0; i < game_objects_.size(); ++i) {
    auto& pos = game_objects_[i].pos;
    int w = map_height * 0.01;
    int h = map_width * 0.01;
    QPoint top = QPoint(((pos.x() - w) * window_width) / map_width,
                        ((pos.y() - h) * window_height) / map_height);
    QPoint
        bottom = QPoint(((pos.x() + w) * window_width) / map_width,
                        ((pos.y() + h) * window_height) / map_height);
    if (game_objects_[i].type == Object::Type::kAttacker) {
      painter.setBrush(Qt::red);
      painter.setPen(QPen(Qt::red, 0));
      painter.drawEllipse(QRect(top, bottom));
    } else if (game_objects_[i].type == Object::Type::kDefender) {
      painter.setBrush(Qt::blue);
      painter.setPen(QPen(Qt::blue, 0));
      painter.drawEllipse(QRect(top, bottom));
    } else if (game_objects_[i].type == Object::Type::kTerrainObject) {
      painter.setBrush(Qt::yellow);
      painter.setPen(QPen(Qt::yellow, 0));
      painter.drawRect(QRect(top, bottom));
    }
  }
  changed_picture_shell_->update();
}

void MapGenerator::ChangeMap() {
  int original_red, delta_red;
  int original_blue, delta_blue;
  int original_green, delta_green;

  for (int i = 0; i < map_.size(); ++i) {
    for (int j = 0; j < map_[i].size(); ++j) {
      original_red = map_[i][j].original_color.red();
      original_green = map_[i][j].original_color.green();
      original_blue = map_[i][j].original_color.blue();

      int64_t precision = -1;
      int64_t p;
      for (int k = 0; k < using_colors_.size(); ++k) {
        delta_red = using_colors_[k].color.red() - original_red;
        delta_green = using_colors_[k].color.green() - original_green;
        delta_blue = using_colors_[k].color.blue() - original_blue;
        p = delta_red * delta_red
            + delta_green * delta_green
            + delta_blue * delta_blue;
        if (precision < 0 || p < precision) {
          map_[i][j].using_color = k;
          precision = p;
        }
      }
    }
  }
}

void MapGenerator::mouseDoubleClickEvent(QMouseEvent* event) {
  int x = event->pos().x();
  int y = event->pos().y();
  if (x >= original_picture_shell_->geometry().x() &&
      y >= original_picture_shell_->geometry().y() &&
      x <= (original_picture_shell_->geometry().x() +
          original_picture_shell_->width()) &&
      y <= (original_picture_shell_->geometry().y() +
          original_picture_shell_->height())) {
    x -= original_picture_shell_->geometry().x();
    y -= original_picture_shell_->geometry().y();

    QPainter painter(&color_shell_->picture_);
    painter.save();

    int i = (map_.size() * y) / (original_picture_shell_->height());
    int j = (map_[i].size() * x) / (original_picture_shell_->width());

    color_ = map_[i][j].original_color;
    painter.setBrush(color_);
    painter.setPen(QPen(color_, 0));
    painter.drawRect(QRect(0,
                           0,
                           painter.window().width(),
                           painter.window().height()));

    SetAdditionalColorVisible(true);

    color_shell_->update();
    painter.restore();
  } else if (x >= using_colors_shell_->geometry().x() &&
      y >= using_colors_shell_->geometry().y() &&
      x <= (using_colors_shell_->geometry().x() +
          using_colors_shell_->width()) &&
      y <= (using_colors_shell_->geometry().y() +
          using_colors_shell_->height())) {
    if (using_colors_.size() == 1) {
      using_colors_[0].color = Qt::white;
      using_colors_[0].speed_characteristic = 0;
      return;
    }
    x -= using_colors_shell_->geometry().x();
    int i = (using_colors_.size() * x) / (using_colors_shell_->width());
    using_colors_.erase(using_colors_.begin() + i);
    DrawUsingColors();
  } else if (x >= changed_picture_shell_->geometry().x() &&
      y >= changed_picture_shell_->geometry().y() &&
      x <= (changed_picture_shell_->geometry().x() +
          changed_picture_shell_->width()) &&
      y <= (changed_picture_shell_->geometry().y() +
          changed_picture_shell_->height())) {
    x -= changed_picture_shell_->geometry().x();
    y -= changed_picture_shell_->geometry().y();
    new_object_pos_.setX((x * map_[0].size()) /
        changed_picture_shell_->width());
    new_object_pos_.setY((y * map_.size()) /
        changed_picture_shell_->height());
    SetSoldiersMenuVisible(true);
  }
}

void MapGenerator::DrawButtonClicked() {
  ConvertImageToArray(QSize(width_->text().toInt(),
                            height_->text().toInt()));
  DrawChangedPicture();
}

void MapGenerator::AddColorButtonClicked() {
  using_colors_.emplace_back(color_,
                             speed_characteristic_->text().toInt());
  DrawUsingColors();

  SetAdditionalColorVisible(false);
}

void MapGenerator::CancelColorButtonClicked() {
  SetAdditionalColorVisible(false);
}

void MapGenerator::AddSoldierButtonClicked() {
  assert(new_object_pos_.y() >= 0 && new_object_pos_.y() < map_.size());
  assert(new_object_pos_.x() >= 0 && new_object_pos_.x() < map_[0].size());
  if (object_variants_->currentText() == "Defender") {
    game_objects_.emplace_back(new_object_pos_,
                               Object::Type::kDefender);
  } else if (object_variants_->currentText() == "Attacker") {
    game_objects_.emplace_back(new_object_pos_,
                               Object::Type::kAttacker);
  } else if (object_variants_->currentText() == "Tower") {
    game_objects_.emplace_back(new_object_pos_,
                               Object::Type::kTerrainObject);
  }
  DrawChangedPicture();
  SetSoldiersMenuVisible(false);
}

void MapGenerator::CancelSoldierButtonClicked() {
  SetSoldiersMenuVisible(false);
}

void MapGenerator::ChangeButtonClicked() {
  ConvertImageToArray(QSize(width_->text().toInt(),
                            height_->text().toInt()));
  ChangeMap();
  using_original_colors = false;
  DrawChangedPicture();
  save_button_->setVisible(true);
}

void MapGenerator::RestoreButtonClicked() {
  using_original_colors = true;
  DrawChangedPicture();
  save_button_->setVisible(false);
}

void MapGenerator::DeleteObjectsButtonClicked() {
  game_objects_.clear();
  DrawChangedPicture();
}

void MapGenerator::SetAdditionalColorVisible(bool visible) {
  color_shell_->setVisible(visible);
  add_color_button_->setVisible(visible);
  cancel_color_button_->setVisible(visible);
  speed_characteristic_->setVisible(visible);
  speed_characteristic_label_->setVisible(visible);
}

void MapGenerator::SetSoldiersMenuVisible(bool visible) {
  add_soldier_button_->setVisible(visible);
  cancel_soldier_button_->setVisible(visible);
  object_variants_->setVisible(visible);
}

void MapGenerator::SaveButtonClicked() {
  assert(!using_original_colors);
  assert(map_.size() > 0 && map_[0].size() > 0);
  QString filter = tr("Text Files (*.json)");
  QString filename = QFileDialog::getOpenFileName(
      this,
      tr("File to save"),
      QDir::currentPath(),
      filter);
  if (filename.isNull()) {
    return;
  }
  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    return;
  }

  QJsonObject record_object;
  QJsonObject map_obj;

  map_obj.insert("Length", static_cast<int>(map_.size()));
  map_obj.insert("Width",  static_cast<int>(map_[0].size()));

  record_object.insert("Size", map_obj);

  QJsonArray colors_and_speed;
  for (int i = 0; i < using_colors_.size(); ++i) {
    QJsonObject obj_color_speed;
    obj_color_speed.insert("Color",
       QJsonValue::fromVariant(
           QVariant::fromValue(using_colors_[i].color.rgb())));

    obj_color_speed.insert("Speed",
                           using_colors_[i].speed_characteristic);

    colors_and_speed.push_back(obj_color_speed);
  }

  record_object.insert("Colors and speed", colors_and_speed);

  QString map_string;

  for (int i = 0; i < map_.size(); ++i) {
    for (int j = 0; j < map_[i].size(); ++j) {
      map_string +=  QString::number(map_[i][j].using_color);
      map_string += " ";
    }
    map_string += "\n";
  }

  record_object.insert("Map", map_string);

  QJsonArray attackers;
  QJsonArray defenders;
  QJsonArray terrain_objects;

  for (int i = 0; i < game_objects_.size(); ++i) {
    auto& object = game_objects_[i];

    QJsonObject obj;
    obj.insert("X", object.pos.x());
    obj.insert("Y", object.pos.y());

    if (object.type == Object::Type::kAttacker) {
      attackers.push_back(obj);
    } else if (object.type == Object::Type::kDefender) {
      defenders.push_back(obj);
    } else if (object.type == Object::Type::kTerrainObject) {
      terrain_objects.push_back(obj);
    }
  }

  record_object.insert("Attackers", attackers);
  record_object.insert("Defenders", defenders);
  record_object.insert("Terrain objects", terrain_objects);

  QJsonDocument doc(record_object);
  QString text  = doc.toJson(QJsonDocument::Indented);

  QTextStream stream(&file);
  stream << text;
  file.close();
}

void MapGenerator::LoadButtonClicked() {
  QString filter = tr("PNG (*.png)");
  QString filename = QFileDialog::getOpenFileName(
      this,
      tr("Image to load"),
      QDir::currentPath(),
      filter);
  if (filename.isNull()) {
    return;
  }

  LoadImageFromFile(filename);
}

void MapGenerator::LoadImageFromFile(const QString& filename) {
  original_picture_shell_->SetPixmap(filename);
  buffer_picture_.load(filename);
  assert(!buffer_picture_.isNull());

  using_original_colors = true;

  SetAdditionalColorVisible(false);
  save_button_->setVisible(false);

  map_.clear();
  ConvertImageToArray(QSize(width_->text().toInt(),
                            height_->text().toInt()));
  using_colors_.clear();
  using_colors_.emplace_back(QColor(255, 255, 255), 0);
  game_objects_.clear();

  DrawButtonClicked();
  DrawUsingColors();
}

MapGenerator::Landscape::Landscape(const QColor& color,
                                   int speed_characteristic) {
  this->color = color;
  this->speed_characteristic = speed_characteristic;
}

MapGenerator::Object::Object(const QPoint& pos,
                             MapGenerator::Object::Type type)
    : type(type), pos(pos) {}

MapGenerator::ImageSHell::ImageSHell(QWidget* parent)
    : QWidget(parent) {
  picture_ = QPixmap(QSize(1000, 1000));
}

void MapGenerator::ImageSHell::SetPixmap(const QPixmap& picture) {
  picture_ = picture;
  this->update();
}

void MapGenerator::ImageSHell::SetPixmap(const QString& path) {
  picture_.load(path);
  this->update();
}

void MapGenerator::ImageSHell::paintEvent(QPaintEvent*) {
  if (picture_.isNull()) {
    return;
  }
  QPainter painter(this);
  painter.save();
  painter.drawPixmap(QRect(0, 0, this->width(), this->height()),
                     picture_);
  painter.restore();
}

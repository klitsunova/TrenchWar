#include "map_generator.h"

MapGenerator::MapGenerator(QWidget* parent)
    : QWidget(parent),
      original_picture_shell_(new ImageSHell(this)),
      changed_picture_shell_(new ImageSHell(this)),
      using_colors_shell_(new ImageSHell(this)),
      color_shell_(new ImageSHell(this)),
      draw_button_(new QPushButton("Draw", this)),
      change_button_(new QPushButton("Change colors", this)),
      restore_button_(new QPushButton("Restore color changes", this)),
      save_button_(new QPushButton("Save color changes", this)),
      load_button_(new QPushButton("Load new image", this)),
      add_color_button_(new QPushButton("<--", this)),
      width_(new QLineEdit("500", this)),
      height_(new QLineEdit("500", this)),
      speed_characteristic_(new QLineEdit("0", this)),
      speed_characteristic_label_(new QLabel("Speed characteristic",
                                             this)) {
  ManageLayout();

  CreateConnections();

  LoadImageFromFile("../map1.png");
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
  layout3->addWidget(add_color_button_);
  layout3->addWidget(color_shell_, 2);

  auto layout4 = new QVBoxLayout();
  layout4->addStretch(1);
  layout4->addWidget(speed_characteristic_label_);
  layout4->addWidget(speed_characteristic_);
  layout4->addStretch(1);

  layout3->addLayout(layout4);

  layout2->addLayout(layout3, 3);
  layout2->addStretch(1);

  auto layout5 = new QVBoxLayout();
  layout5->addStretch(1);
  layout5->addWidget(load_button_);
  layout5->addStretch(1);
  auto label1 = new QLabel("Width", this);
  layout5->addWidget(label1, 0);
  layout5->addWidget(width_, 0);
  auto label2 = new QLabel("Height", this);
  layout5->addWidget(label2, 0);
  layout5->addWidget(height_, 0);
  layout5->addStretch(1);
  layout5->addWidget(draw_button_, 0);
  layout5->addWidget(change_button_, 0);
  layout5->addWidget(restore_button_, 0);
  layout5->addWidget(save_button_, 0);
  layout5->addStretch(1);

  auto layout6 = new QHBoxLayout(this);
  layout6->addStretch(1);
  layout6->addLayout(layout2, 30);
  layout6->addStretch(1);
  layout6->addLayout(layout5, 0);
  layout6->addStretch(1);
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
  connect(change_button_,
          &QPushButton::clicked,
          this,
          &MapGenerator::ChangeButtonClicked);
  connect(restore_button_,
          &QPushButton::clicked,
          this,
          &MapGenerator::RestoreButtonClicked);
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
  QPainter painter(&changed_picture_shell_->picture_);
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
  if (x >= original_picture_shell_->x() &&
      y >= original_picture_shell_->y() &&
      x <= (original_picture_shell_->x() +
          original_picture_shell_->width()) &&
      y <= (original_picture_shell_->y() +
          original_picture_shell_->height())) {
    x -= original_picture_shell_->x();
    y -= original_picture_shell_->y();

    SetAdditionalColorVisible(true);

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
    color_shell_->update();
    painter.restore();
  } else if (x >= using_colors_shell_->x() &&
      y >= using_colors_shell_->y() &&
      x <= (using_colors_shell_->x() +
          using_colors_shell_->width()) &&
      y <= (using_colors_shell_->y() +
          using_colors_shell_->height())) {
    if (using_colors_.size() == 1) {
      using_colors_[0].color = Qt::white;
      using_colors_[0].speed_characteristic = 0;
      return;
    }
    x -= using_colors_shell_->x();
    int i = (using_colors_.size() * x) / (using_colors_shell_->width());
    using_colors_.erase(using_colors_.begin() + i);
    DrawUsingColors();
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

void MapGenerator::SetAdditionalColorVisible(bool visible) {
  color_shell_->setVisible(visible);
  add_color_button_->setVisible(visible);
  speed_characteristic_->setVisible(visible);
  speed_characteristic_label_->setVisible(visible);
}

void MapGenerator::SaveButtonClicked() {
  assert(!using_original_colors);
  assert(map_.size() > 0 && map_[0].size() > 0);
  QString filter = tr("Text Files (*.txt)");
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
  QTextStream fout(&file);
  fout << using_colors_.size() << "\n";
  for (int i = 0; i < using_colors_.size(); ++i) {
    fout << using_colors_[i].color.rgb() << " "
         << using_colors_[i].speed_characteristic << "\n";
  }
  fout << "\n" << map_.size() << " " << map_[0].size() << "\n";
  for (int i = 0; i < map_.size(); ++i) {
    for (int j = 0; j < map_[i].size(); ++j) {
      fout << map_[i][j].using_color << " ";
    }
    fout << "\n";
  }
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

  using_original_colors = true;

  SetAdditionalColorVisible(false);
  save_button_->setVisible(false);

  map_.clear();
  using_colors_.clear();
  using_colors_.emplace_back(Landscape(QColor(255, 255, 255), 0));

  DrawButtonClicked();
  DrawUsingColors();
}

MapGenerator::Landscape::Landscape(const QColor& color,
                                   int speed_characteristic) {
  this->color = color;
  this->speed_characteristic = speed_characteristic;
}

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

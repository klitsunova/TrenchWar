#pragma once

#include <QApplication>
#include <QPainter>
#include <QPushButton>
#include <QImage>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QColor>
#include <QString>
#include <QFileDialog>
#include <QStringList>

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

class MapGenerator : public QWidget {
  Q_OBJECT;

 private:
  struct ladscape;
  class ImageShell;

 public:
  explicit MapGenerator(QWidget* parent = nullptr);

  void mouseDoubleClickEvent(QMouseEvent*) override;

 private:
  struct ChangingColor {
    int using_color{0};
    QColor original_color{0xffffff};
  };

  struct Landscape {
    Landscape(const QColor&, int);
    QColor color{0xffffff};
    int speed_characteristic{0};
  };

  class ImageSHell : public QWidget {
   public:
    explicit ImageSHell(QWidget* parent = nullptr);

    void SetPixmap(const QPixmap&);
    void SetPixmap(const QString&);

    void paintEvent(QPaintEvent*) override;

   private:
    friend class MapGenerator;
    QPixmap picture_;
  };

  void ManageLayout();

  void CreateConnections();

  void SetAdditionalColorVisible(bool);

  void DrawChangedPicture();
  void DrawUsingColors();

  void ConvertImageToArray(const QSize&);

  void ChangeMap();

  void DrawButtonClicked();
  void AddColorButtonClicked();
  void ChangeButtonClicked();
  void RestoreButtonClicked();
  void SaveButtonClicked();
  void LoadButtonClicked();

  void LoadImageFromFile(const QString&);

  std::vector<std::vector<ChangingColor>> map_;
  bool using_original_colors;
  std::vector<Landscape> using_colors_;
  QImage buffer_picture_;
  ImageSHell* original_picture_shell_;
  ImageSHell* changed_picture_shell_;
  ImageSHell* using_colors_shell_;
  ImageSHell* color_shell_;
  QColor color_;
  QPushButton* draw_button_;
  QPushButton* change_button_;
  QPushButton* restore_button_;
  QPushButton* save_button_;
  QPushButton* load_button_;
  QPushButton* add_color_button_;
  QLineEdit* width_;
  QLineEdit* height_;
  QLineEdit* speed_characteristic_;
  QLabel* speed_characteristic_label_;
};

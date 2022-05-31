#pragma once

#include <QApplication>
#include <QComboBox>
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

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>

class MapGenerator : public QWidget {
  Q_OBJECT;

 private:
  struct Ladscape;
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

  struct Object{
    enum class Type{
      kDefender,
      kAttacker,
      kTerrainObject
    };
    Object(const QPoint&, Type);
    Type type;
    QPoint pos;
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
  void SetSoldiersMenuVisible(bool);

  void DrawChangedPicture();
  void DrawUsingColors();
  void DrawObjects();

  void ConvertImageToArray(const QSize&);

  void ChangeMap();

  void DrawButtonClicked();
  void AddColorButtonClicked();
  void CancelColorButtonClicked();
  void AddSoldierButtonClicked();
  void CancelSoldierButtonClicked();
  void ChangeButtonClicked();
  void RestoreButtonClicked();
  void DeleteObjectsButtonClicked();
  void SaveButtonClicked();
  void LoadButtonClicked();

  void LoadImageFromFile(const QString&);

  std::vector<std::vector<ChangingColor>> map_;
  bool using_original_colors;
  std::vector<Landscape> using_colors_;
  std::vector<Object> game_objects_;
  QImage buffer_picture_;
  ImageSHell* original_picture_shell_;
  ImageSHell* changed_picture_shell_;
  ImageSHell* using_colors_shell_;
  ImageSHell* color_shell_;
  QColor color_;
  QPushButton* draw_button_;
  QPushButton* change_button_;
  QPushButton* restore_button_;
  QPushButton* delete_objects_button_;
  QPushButton* save_button_;
  QPushButton* load_button_;
  QPushButton* add_color_button_;
  QPushButton* cancel_color_button_;
  QPushButton* add_soldier_button_;
  QPushButton* cancel_soldier_button_;
  QLineEdit* width_;
  QLineEdit* height_;
  QLineEdit* speed_characteristic_;
  QLabel* speed_characteristic_label_;
  QPoint new_object_pos_;
  QComboBox* object_variants_;
  QPixmap attacker_;
  QPixmap defender_;
  QPixmap tower_;
};

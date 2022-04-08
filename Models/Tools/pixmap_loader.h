#pragma once

#include <QApplication>
#include <QPixmap>
#include <QString>

#include <iostream>
#include <cassert>
#include <map>
#include <memory>

class PixmapLoader {
 public:

  static void LoadPixmap(const QString&);

  static const std::shared_ptr<QPixmap>& GetPixmap(const QString&);

  static const std::shared_ptr<QPixmap>& GetSoldier();
  static const std::shared_ptr<QPixmap>& GetLSoldier();
  static const std::shared_ptr<QPixmap>& GetRSoldier();

  static const std::shared_ptr<QPixmap>& GetTerrainObject();

 private:
  PixmapLoader() = default;

  static std::map<QString, std::shared_ptr<QPixmap>> images;
  static QString soldier_path;
  static QString right_soldier_path;
  static QString left_soldier_path;
  static QString terrain_object_path;
};


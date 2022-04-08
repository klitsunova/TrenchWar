#pragma once

#include <QApplication>
#include <QPixmap>
#include <QString>

#include <iostream>
#include <cassert>
#include <map>

class PixmapLoader {
 public:
  PixmapLoader();

  ~PixmapLoader() = default;

  void LoadPixmap(const QString&);

  const std::shared_ptr<QPixmap>& GetPixmap(const QString&);

  const std::shared_ptr<QPixmap>& GetSoldier();
  const std::shared_ptr<QPixmap>& GetLSoldier();
  const std::shared_ptr<QPixmap>& GetRSoldier();

  const std::shared_ptr<QPixmap>& GetTerrainObject();

 private:
  std::map<QString, std::shared_ptr<QPixmap>> images_;
  QString soldier_path_;
  QString right_soldier_path_;
  QString left_soldier_path_;
  QString terrain_object_path_;
};

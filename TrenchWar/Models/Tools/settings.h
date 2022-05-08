#pragma once

#include <QSettings>
#include <QObject>

class Settings : public QObject {
  Q_OBJECT
 public:
  static constexpr int min_volume = 0;
  static constexpr int max_volume = 100;
  static constexpr int start_volume = 50;
  static constexpr bool fullscreen = true;

  Settings(const Settings&) = delete;
  Settings& operator=(Settings&) = delete;

  static Settings* getInstance() {
    if(!instance_) {
      instance_ = new Settings();
    }
    return instance_;
  }

  void SetMusicVolume(int volume);
  int GetMusicVolume();

  void SetEffectsVolume(int volume);
  int GetEffectsVolume();

  void SetFullScreenValue(bool IsFullScreen);
  bool IsFullScreen();

  signals:

 private:
  Settings();
  static Settings* instance_;

  QSettings settings_;
};

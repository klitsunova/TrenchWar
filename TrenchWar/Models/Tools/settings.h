#pragma once

#include <QSettings>

class Settings {
 public:
  static constexpr int min_volume = 0;
  static constexpr int max_volume = 100;

  Settings(const Settings&) = delete;
  Settings& operator=(Settings&) = delete;

  static Settings* getInstance() {
    if(!instance_) {
      instance_ = new Settings;
    }
    return instance_;
  }

  void SetMusicVolume(int volume);

  int GetMusicVolume();
  void SetEffectsVolume(int volume);

  int GetEffectsVolume();
  void SetFullScreenValue(bool IsFullScreen);

  bool IsFullScreen();

 private:
  Settings() = default;
  static Settings* instance_;

  QSettings settings_{};
};

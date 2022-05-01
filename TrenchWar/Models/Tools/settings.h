#pragma once

#include <QSettings>

class Settings {
 public:
  static constexpr int min_volume = 0;
  static constexpr int max_volume = 100;

 public:
  Settings() = default;

  ~Settings() = default;
  void SetMusicVolume(int volume);

  int GetMusicVolume();
  void SetEffectsVolume(int volume);

  int GetEffectsVolume();
  void SetFullScreenValue(bool IsFullScreen);

  bool IsFullScreen();

 private:
  QSettings settings_;
};

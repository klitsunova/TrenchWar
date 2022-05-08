#pragma once

#include <QSettings>
#include <QObject>

class Settings : public QObject {
  Q_OBJECT

 public:
  static constexpr int kMinVolume = 0;
  static constexpr int kMaxVolume = 100;
  static constexpr int kStartVolume = 50;
  static constexpr bool kFullScreen = true;

  Settings(const Settings&) = delete;
  Settings& operator=(Settings&) = delete;

  static Settings* getInstance() {
    if (instance_ == nullptr) {
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

 private:
  Settings();
  static Settings* instance_;

  QSettings settings_{};
};

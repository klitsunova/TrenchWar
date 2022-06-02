#pragma once

#include <QObject>
#include <QSettings>

class Settings : public QObject {
  Q_OBJECT

 public:
  static constexpr int kMinVolume = 0;
  static constexpr int kMaxVolume = 100;
  static constexpr int kStartVolume = 50;
  static constexpr bool kFullScreen = true;

  Settings() = delete;
  Settings(const Settings&) = delete;
  Settings& operator=(Settings&) = delete;

  // static Settings* Instance() {
  //   if (instance_ == nullptr) {
  //     instance_ = new Settings();
  //   }
  //   return instance_;
  // }

  static void SetMusicVolume(int volume);
  static int GetMusicVolume();

  static void SetEffectsVolume(int volume);
  static int GetEffectsVolume();

  static void SetFullScreenValue(bool IsFullScreen);
  static bool IsFullScreen();

  static void SetStandardSettings();

 private:
};

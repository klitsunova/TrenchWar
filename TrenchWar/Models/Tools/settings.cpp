#include <QString>

#include "settings.h"

namespace {
  const QString kKeyMusicVolume = "music_volume";
  const QString kKeyEffectsVolume = "effects_volume";
  const QString kKeyFullscreen = "is_fullscreen";
}

Settings* Settings::instance_ = nullptr;

void Settings::SetMusicVolume(int volume) {
  settings_.setValue(kKeyMusicVolume, volume);
}

int Settings::GetMusicVolume() {
  return settings_.value(kKeyMusicVolume).toInt();
}

void Settings::SetEffectsVolume(int volume) {
  settings_.setValue(kKeyEffectsVolume, volume);
}

int Settings::GetEffectsVolume() {
  return settings_.value(kKeyEffectsVolume).toInt();
}

void Settings::SetFullScreenValue(bool IsFullScreen) {
  settings_.setValue(kKeyFullscreen, IsFullScreen);
}

bool Settings::IsFullScreen() {
  return settings_.value(kKeyFullscreen).toBool();
}

Settings::Settings() {
  // TODO(Zolokinos) make default values. Or save in INI.
  // SetMusicVolume(start_volume);
  // SetFullScreenValue(fullscreen);
}

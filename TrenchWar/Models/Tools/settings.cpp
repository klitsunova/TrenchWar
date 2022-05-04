#include "settings.h"

#include <QString>

namespace {
  const QString key_music_volume = "music_volume";
  const QString key_effects_volume = "effects_volume";
  const QString key_fullscreen = "is_fullscreen";
}

void Settings::SetMusicVolume(int volume) {
  settings_.setValue(key_music_volume, volume);
}

int Settings::GetMusicVolume() {
  return settings_.value(key_music_volume).toInt();
}

void Settings::SetEffectsVolume(int volume) {
  settings_.setValue(key_effects_volume, volume);
}

int Settings::GetEffectsVolume() {
  return settings_.value(key_effects_volume).toInt();
}

void Settings::SetFullScreenValue(bool IsFullScreen) {
  settings_.setValue(key_fullscreen, IsFullScreen);
}

bool Settings::IsFullScreen() {
  return settings_.value(key_fullscreen).toBool();
}

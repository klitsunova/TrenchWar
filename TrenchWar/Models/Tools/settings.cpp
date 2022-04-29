#include "settings.h"

void Settings::SetMusicVolume(int volume) {
  QSettings settings;
  settings.setValue("music_volume", volume);
}

int Settings::GetMusicVolume() {
  return settings_.value("music_volume").toInt();
}

void Settings::SetEffectsVolume(int volume) {
  settings_.setValue("effects_volume", volume);
}

int Settings::GetEffectsVolume() {
  return settings_.value("effects_volume").toInt();
}

void Settings::SetFullScreenValue(bool IsFullScreen) {
  settings_.setValue("is_fullscreen", IsFullScreen);
}

bool Settings::IsFullScreen() {
  return settings_.value("is_fullscreen").toBool();
}

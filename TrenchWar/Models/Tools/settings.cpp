#include <QString>

#include "settings.h"

namespace {
const QString kOrganization = "ESD_Team";
const QString kApplication = "Trench war";
const QString kKeyMusicVolume = "music_volume";
const QString kKeyEffectsVolume = "effects_volume";
const QString kKeyFullscreen = "is_fullscreen";
}

void Settings::SetMusicVolume(int volume) {
  QSettings settings(kOrganization, kApplication);
  settings.setValue(kKeyMusicVolume, volume);
}

int Settings::GetMusicVolume() {
  QSettings settings(kOrganization, kApplication);
  return settings.value(kKeyMusicVolume).toInt();
}

void Settings::SetEffectsVolume(int volume) {
  QSettings settings(kOrganization, kApplication);
  settings.setValue(kKeyEffectsVolume, volume);
}

int Settings::GetEffectsVolume() {
  QSettings settings(kOrganization, kApplication);
  return settings.value(kKeyEffectsVolume).toInt();
}

void Settings::SetFullScreenValue(bool IsFullScreen) {
  QSettings settings(kOrganization, kApplication);
  settings.setValue(kKeyFullscreen, IsFullScreen);
}

bool Settings::IsFullScreen() {
  QSettings settings(kOrganization, kApplication);
  return settings.value(kKeyFullscreen).toBool();
}

void Settings::SetStandardSettings() {
  // TODO(AZYAVCHIKOV): maybe not best settings
  SetMusicVolume(50);
  SetFullScreenValue(false);
  SetEffectsVolume(50);
}

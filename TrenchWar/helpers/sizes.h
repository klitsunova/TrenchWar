#pragma once

#include <QSize>

namespace window_sizes {
constexpr QSize kMenu(1280, 600);
constexpr QSize kSettingsMenu(800, 600);
constexpr QSize kPauseMenu(400, 400);
constexpr QSize kDialogWidget(350, 200);
// TODO(Zolokinos) Move away
constexpr QSize kWorld(500, 400);
}  // namespace window_sizes

namespace element_sizes {
constexpr QSize kMenuButton(300, 75);
constexpr QSize kDialogButton(150, 50);
constexpr QSize kStore(1000, 150);
constexpr QSize kStoreBox(70, 70);
constexpr QSize kTrenchBuild(70, 70);
}  // namespace element_sizes

namespace interval_sizes {
constexpr int kSpacing(15);
constexpr int kStretch(10);
}  // namespace interval_sizes

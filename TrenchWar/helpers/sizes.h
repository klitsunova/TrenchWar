#pragma once

#include <QSize>

namespace window_sizes {
constexpr QSize kMenu(1280, 800);
constexpr QSize kSettingsMenu(800, 600);
constexpr QSize kPauseMenu(400, 400);
constexpr QSize kDialogWidget(350, 200);
constexpr QSize kWorld(1000, 1000);
}  // namespace window_sizes

namespace element_sizes {
constexpr QSize kMenuButton(300, 75);
constexpr QSize kDialogButton(150, 50);
}  // namespace element_sizes

namespace interval_sizes {
constexpr int kSpacing(15);
constexpr int kStretch(10);
}  // namespace interval_sizes
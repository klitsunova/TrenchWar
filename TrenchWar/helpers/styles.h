#pragma once

#include <QString>

namespace styles {
const QString kPushButtonStyle =
    "QPushButton {"
    "background-color: #9AA670;"
    "border-style: outset;"
    "border-width: 5px;"
    "border-radius: 10px;"
    "border-color: #1c2f0c;"
    "padding: 5px;"
    "font: 24px;"
    "color: #172E09; }"

    "QPushButton::pressed {"
    "background-color: #233b0f;"
    "border-style: inset; }";
} // namespace styles

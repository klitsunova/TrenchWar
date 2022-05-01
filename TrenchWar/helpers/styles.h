#pragma once

#include <QString>

namespace styles {
const QString kPushButton =
    "QPushButton {"
    "background-color: #9AA670;"
    "border-style: outset;"
    "border-width: 5px;"
    "border-radius: 10px;"
    "border-color: #1C2F0C;"
    "padding: 5px;"
    "color: #172E09; }"

    "QPushButton::pressed {"
    "background-color: #233b0f;"
    "border-style: inset; }";

const QString kExitButton =
    "QPushButton {"
    "background-color: #A85E5E;"
    "border-style: outset;"
    "border-width: 5px;"
    "border-radius: 10px;"
    "border-color: #3A2323;"
    "padding: 5px;"
    "color: #172E09; }"

    "QPushButton::pressed {"
    "background-color: #6B3333;"
    "border-style: inset; }";

const QString kWidget =
    "QWidget {"
    "background-color: #BFB790;}";
}  // namespace styles

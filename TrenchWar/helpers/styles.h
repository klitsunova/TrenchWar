#pragma once

#include <QString>
#include <QWidget>

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

const QString kStoreBox =
    "QLabel {"
    "border: 2px solid green;"
    "border-radius: 4px;"
    "padding: 2px;"
    "background-color: #BFB790;}";

const QString kLineEdit =
    "QLineEdit {"
    "border: 2px solid black;"
    "background-color: #DFDAC1;"
    "border-radius: 10px;"
    "padding: 0 8px;"
    "font: 15px;"
    "selection-background-color: darkgray; }";
}  // namespace styles

namespace colors {
constexpr QColor kTrenchColor{92, 64, 51};
}

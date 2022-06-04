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
    "border-style: inset; }"

    "QPushButton:disabled {"
    "border-color: #233b0f;}"

    "QPushButton:hover {"
    "border-color: black;}";

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

const QString kStoreBoxMoneyLabels =
    "QWidget {"
    "border: 0px solid green;"
    "border-radius: 4px;"
    "padding: 2px;"
    "background-color: #BFB790;}";

const QString kStoreBoxMoneyWidget =
    "QWidget {"
    "border: 2px solid green;"
    "border-radius: 4px;"
    "padding: 2px;"
    "background-color: #BFB790;}";

const QString kStoreMenu =
    "QWidget {"
    "background-color: #40826d;}";

const QString kStoreDialog =
    "QDialog {"
    "background-color: white}";

const QString kRadioButton =
    "QRadioButton::indicator {"
    "width: 30px;"
    "height: 30px;}"

    "QRadioButton::indicator::unchecked {"
    "image: url(:/Resources/Images/UncheckedCommon.png);}"

    "QRadioButton::indicator::checked {"
    "image: url(:/Resources/Images/CheckedCommon.png);}"

    "QRadioButton::indicator::unchecked:hover {"
    "image: url(:/Resources/Images/UncheckedHovered.png);}"

    "QRadioButton::indicator::checked:hover {"
    "image: url(:/Resources/Images/CheckedHovered.png);}"

    "QRadioButton::indicator::unchecked:pressed {"
    "image: url(:/Resources/Images/UncheckedPressed.png);}"

    "QRadioButton::indicator::checked:pressed {"
    "image: url(:/Resources/Images/CheckedPressed.png);}";

const QString kCheckBox =
    "QCheckBox {"
    "spacing: 5px;}"

    "QCheckBox::indicator {"
    "width: 30px;"
    "height: 30px;}"

    "QCheckBox::indicator::unchecked {"
    "image: url(:/Resources/Images/UncheckedCommon.png);}"

    "QCheckBox::indicator::checked {"
    "image: url(:/Resources/Images/CheckedCommon.png);}"

    "QCheckBox::indicator::unchecked:hover {"
    "image: url(:/Resources/Images/UncheckedHovered.png);}"

    "QCheckBox::indicator::checked:hover {"
    "image: url(:/Resources/Images/CheckedHovered.png);}"

    "QCheckBox::indicator::unchecked:pressed {"
    "image: url(:/Resources/Images/UncheckedPressed.png);}"

    "QCheckBox::indicator::checked:pressed {"
    "image: url(:/Resources/Images/CheckedPressed.png);}";

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

import QtQuick
import QtQuick.Controls

TextField {
    id: textField

    readonly property alias intValidator: intValidator

    inputMethodHints: Qt.ImhDigitsOnly
    validator: intValidator

    IntValidator { id: intValidator }
}



import QtQuick
import QtQuick.Controls

TextField {
    inputMethodHints: Qt.ImhDigitsOnly
    validator: IntValidator {}
}

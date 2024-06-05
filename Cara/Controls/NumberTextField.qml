import QtQuick
import QtQuick.Controls

Item {
    readonly property alias intValidator: intValidator
    property alias placeholderText: textField.placeholderText
    property alias text: textField.text
    property alias inputMethodHints: textField.inputMethodHints

    implicitHeight: textField.implicitHeight
    TextField {
        id: textField
        anchors.fill: parent
        inputMethodHints: Qt.ImhDigitsOnly
        validator: intValidator
    }

    IntValidator { id: intValidator }
}

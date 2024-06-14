import QtQuick
import QtQuick.Controls
import Cara.Controls

Item {
    id: root

    property alias font: textField.font
    property alias text: textField.text
    property alias inputMask: textField.inputMask
    property alias placeholderText: textField.placeholderText
    property alias validator: textField.validator
    property date currentDate
    readonly property date displayDate: Date.fromLocaleDateString(Qt.locale(), root.text, "dd/MM/yyyy")
    readonly property alias acceptableInput: textField.acceptableInput
    readonly property bool empty: textField.text.length <= 2

    implicitWidth: textField.implicitWidth
    implicitHeight: textField.implicitHeight

    TextField {
        id: textField
        anchors.fill: parent
        inputMask: "99/99/9999"
        text: if (root.currentDate) root.currentDate.toLocaleDateString(Qt.locale(), "dd/MM/yyyy")
    }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    property alias placeholderText: textField.placeholderText
    readonly property string toFind: textField.text
    property bool suggetions: true

    signal accepted(string text)

    spacing: 10
    width: parent.width
    height: parent.height * .2

    Keys.onReturnPressed: accepted(textField.text)
    Keys.onEnterPressed: accepted(textField.text)

    TextField {
        id: textField
        focus: true
        Layout.fillWidth: true
        //Keys.onReturnPressed: shouldFind()
        Layout.alignment: Qt.AlignTop
        Layout.margins: 5
        Layout.leftMargin: 5
    }

    ToolButton {
        background: Image {
            antialiasing: false
            source: "qrc:/icons/search.svg"
            sourceSize: iconSize
        }
        onClicked: accepted(text)
        Layout.alignment: Qt.AlignTop
        Layout.margins: 0
        Layout.rightMargin: 10
    }

    function hide() {
        //TODO: Implement this
    }

    function show() {
        //TODO: Implement this
    }
}

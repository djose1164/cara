import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    property alias placeholderText: textField.placeholderText
    readonly property string toFind: textField.text
    
    signal find

    spacing: 10
    width: parent.width
    height: parent.height * .2

    TextField {
        id: textField
        focus: true
        Layout.fillWidth: true
        Keys.onReturnPressed: shouldFind()
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
        onClicked: shouldFind()
        Layout.alignment: Qt.AlignTop
        Layout.margins: 0
        Layout.rightMargin: 10
    }

    function shouldFind() {
        if (toFind)
            find();
    }
}

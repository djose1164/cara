import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ToolBar {
    property alias placeholderText: textField.placeholderText
    readonly property string toFind: textField.text
    property bool suggetions: true

    signal accepted(string text)

    id: root

    RowLayout {
        spacing: 10

        Keys.onReturnPressed: accepted(textField.text)
        Keys.onEnterPressed: accepted(textField.text)

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
            background: IconImage {
                source: "qrc:/icons/settings.svg"
                sourceSize: iconSize
            }
            Layout.alignment: Qt.AlignRight
        }
    }

    function hide() {
        //TODO: Implement this
    }

    function show() {
        //TODO: Implement this
    }
}

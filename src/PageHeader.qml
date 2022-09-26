import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ToolBar {
    required property string title
    required property var stackView

    RowLayout {
        anchors.fill: parent

        ToolButton {
            Layout.alignment: Qt.AlignLeading
            Layout.margins: 5
            background: Image {
                source: "qrc:/icons/left_arrow.svg"
                sourceSize: iconSize
                antialiasing: false
            }

            onClicked: stackView.pop()
        }

        Label {
            text: title
        }
    }
}

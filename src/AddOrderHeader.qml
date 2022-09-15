import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ToolBar {
    id: headerToolBar
    RowLayout {
        anchors.fill: parent

        ToolButton {
            background: Image {
                source: "qrc:/icons/left_arrow.svg"
                antialiasing: false
                sourceSize: iconSize
            }
            Layout.alignment: Qt.AlignLeft
            onClicked: mainStackView.pop()
        }

        Label {
            text: "Ordenes"
            font.bold: true
            Layout.alignment: Qt.AlignCenter
        }

        ToolButton {
            background: Image {
                source: "qrc:/icons/filter.svg"
                antialiasing: false
                sourceSize: iconSize
            }
            Layout.alignment: Qt.AlignRight
        }
    }
}

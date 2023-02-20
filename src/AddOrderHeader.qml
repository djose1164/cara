import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ToolBar {
    id: headerToolBar
    RowLayout {
        anchors.fill: parent

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

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"

ToolBar {
    RowLayout {
        anchors.fill: parent

        ToolButton {
            background: Image {
                source: "qrc:/icons/left_arrow.svg"
                sourceSize: iconSize
            }
            onClicked: mainStackView.pop()
            Layout.topMargin: 5
        }

        Label {
            text: "Facturas"
            font.bold: true
            font.pointSize: 18
            Layout.alignment: Qt.AlignCenter
            Layout.topMargin: 5
        }

        ToolButton {
            background: Image {
                source: "qrc:/icons/settings.svg"
                sourceSize: iconSize
            }
            onClicked: filter.open()
            Layout.alignment: Qt.AlignRight
            Layout.topMargin: 5
            Layout.rightMargin: 5
        }
    }

    Filter { id: filter }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ToolBar {
    RowLayout {
        anchors.fill: parent

        Label {
            text: "Ordenes"
            font.bold: true
            Layout.fillWidth: true
            horizontalAlignment: Label.AlignHCenter
        }

        ToolButton {
            icon.source: "qrc:/icons/filter.svg"
            Layout.alignment: Qt.AlignTrailing
        }
    }
}

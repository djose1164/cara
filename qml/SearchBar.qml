import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"

ToolBar {
    id: root

    readonly property alias loader: loader

    signal backClicked

    RowLayout {
        anchors.fill: parent

        BackButton {
            Layout.alignment: Qt.AlignLeading
            onClicked: backClicked()
        }

        Loader {
            id: loader
            Layout.maximumHeight: root.height*.75
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter
            Layout.rightMargin: root.width*.1
        }
    }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Utils

ToolBar {
    id: root

    readonly property alias loader: loader

    signal backClicked

    background: Rectangle {
        color: "floralwhite"
    }

    RowLayout {
        anchors.fill: parent

        BackButton {
            icon.color: "black"
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

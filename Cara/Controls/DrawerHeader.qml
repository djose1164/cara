import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cara.Controls

Rectangle {
    id: root

    property string title
    property bool midBtnVisible: true
    readonly property alias loader: menuLoader

    signal closeClicked

    implicitWidth: rowLayout.implicitWidth
    implicitHeight: rowLayout.implicitHeight
    z: 100
    color: "white"

    RowLayout {
        id: rowLayout
        anchors.fill: parent

        CloseButton {
            Layout.alignment: Qt.AlignLeading | Qt.AlignVCenter
            onClicked: root.closeClicked()
        }

        Label {
            visible: root.midBtnVisible
            text: root.title
            font.pointSize: 14
            font.bold: true
            Layout.alignment: Qt.AlignLeading | Qt.AlignVCenter
        }

        Loader {
            id: menuLoader
            Layout.fillWidth: !root.midBtnVisible
            Layout.maximumHeight: root.height
        }
    }
}

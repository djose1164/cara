import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Cara.Utils

Rectangle {
    id: root

    property string title
    property bool midBtnVisible: true
    readonly property alias loader: menuLoader

    signal closeClicked

    height: 50
    z: 100
    color: "white"

    RowLayout {
        anchors.fill: parent

        CloseButton {
            Layout.alignment: Qt.AlignLeading | Qt.AlignVCenter
            onClicked: root.closeClicked()
        }

        Label {
            visible: midBtnVisible
            text: root.title
            font.pointSize: 14
            font.bold: true
            Layout.alignment: Qt.AlignLeading | Qt.AlignVCenter
        }

        Loader {
            id: menuLoader
            Layout.fillWidth: !midBtnVisible
            Layout.maximumHeight: root.height
        }
    }
}

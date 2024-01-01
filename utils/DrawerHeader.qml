import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import core
import "qrc:/utils"

Rectangle {
    id: root

    property string title
    readonly property alias menu: menuLoader

    height: 50
    z: 100
    color: "white"

    RowLayout {
        anchors.fill: parent

        CloseButton {
            onClicked: close()
            Layout.alignment: Qt.AlignLeading | Qt.AlignVCenter
        }

        Label {
            text: root.title
            font.pointSize: 14
            font.bold: true
            Layout.alignment: Qt.AlignLeading | Qt.AlignVCenter
        }

        Loader {
            id: menuLoader
        }
    }
}

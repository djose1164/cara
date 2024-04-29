import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property alias title: label.text
    readonly property alias loader: loader

    signal closeClicked

    implicitWidth: rowLayout.implicitWidth
    implicitHeight: 50

    RowLayout {
        id: rowLayout
        anchors.fill: parent

        CloseButton {
            onClicked: root.closeClicked()
        }

        Label {
            id: label
            text: "Title"
            font.pointSize: 16
            font.bold: true
            Layout.fillWidth: true
        }

        Loader {
            id: loader
            Layout.alignment: Qt.AlignRight
        }
    }
}

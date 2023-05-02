import QtQuick
import QtQuick.Controls
import "qrc:/utils" as Utils

Utils.CaDrawer {
    width: parent.width
    height: parent.height
    edge: Qt.BottomEdge
    title: "Codigo QR"

    TextField {
        anchors.top: parent.top
        anchors.topMargin: 20
        id: inputField
        text: "Hello world!"
    }

}

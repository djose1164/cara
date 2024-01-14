import QtQuick
import QtQuick.Controls

Popup {
    width: parent.width * .7
    height: parent.height/2
    anchors.centerIn: parent
    modal: true
    focus: true
    background: Rectangle {
        color: "transparent"
    }

    BusyIndicator {
        anchors.fill: parent
    }
}

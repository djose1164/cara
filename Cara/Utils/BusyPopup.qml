import QtQuick
import QtQuick.Controls

Popup {
    width: 200
    height: 200
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

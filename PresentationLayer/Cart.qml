import QtQuick
import QtQuick.Controls

Page {
    id: root

    Rectangle {
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        height: 75
        width: parent.width
        color: "blue"
    }

    CartContent {
        id: cartContent
        anchors.top: parent.top
        anchors.topMargin: 80
        height: root.height-100
        width: root.width
    }

}

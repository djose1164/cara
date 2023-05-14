import QtQuick
import QtQuick.Controls

Rectangle {
    id: root

    property alias imgSrc: img.source
    property alias nameText: nameField.text
    property alias nameEnabled: nameField.enabled

    signal imageReady

    width: 400
    height: childrenRect.height
    border.width: 1
    radius: 10

    Column {
        id: column
        spacing: 20
        bottomPadding: 30

        Item {
            width: root.width
            height: img.height

            ProductImage {
                id: img
                anchors.centerIn: parent
                sourceSize: "256x256"
                onReady: imageReady()
            }
        }

        ToolSeparator { width: root.width; orientation: Qt.Horizontal }

        TextEdit {
            id: nameField
            anchors.left: parent.left
            anchors.leftMargin: 10
            font.pointSize: 14
        }
    }
}

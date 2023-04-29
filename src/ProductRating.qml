import QtQuick
import QtQuick.Controls
import "qrc:/utils"

Rectangle {
    id: root

    property alias imgSrc: img.source
    property alias selectedStars: starRating.selectedStars
    property alias nameText: nameField.text
    property alias nameEnabled: nameField.enabled

    width: 400
    height: childrenRect.height
    border.width: 1
    radius: 10

    Column {
        id: column
        spacing: 20
        bottomPadding: 10

        Item {
            width: root.width
            height: img.height

            ProductImage {
                id: img
                anchors.centerIn: parent
                sourceSize: "256x256"
            }
        }

        ToolSeparator { width: root.width; orientation: Qt.Horizontal }

        StarRating {
            id: starRating
            anchors.left: parent.left
            anchors.leftMargin: 10
            enabled: false
        }

        TextEdit {
            id: nameField
            anchors.left: parent.left
            anchors.leftMargin: 10
            font.pointSize: 14
        }
    }
}

import QtQuick
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
        leftPadding: 10
        bottomPadding: 10

        ProductImage {
            id: img
            sourceSize: `${256}x${256}`
        }

        StarRating {
            id: starRating
            width: root.width/2
            height: 50
            enabled: false
        }

        TextEdit {
            id: nameField
            font.pointSize: 14
        }
    }
}

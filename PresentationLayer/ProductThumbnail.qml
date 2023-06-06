import QtQuick
import QtQuick.Controls
import "qrc:/product"

Rectangle {
    id: root
    property alias name: name.text
    property alias detailed: detailed.text
    property alias source: productImage.source

    width: 200
    height: 70

    ProductImage {
        id: productImage
        anchors.left: root.left
        anchors.leftMargin: 5
        anchors.verticalCenter: root.verticalCenter
        sourceSize: "100x100"
    }

    Column {
        anchors.left: productImage.right
        anchors.leftMargin: 10

        Label {
            id: name
            font.bold: true
        }

        Label {
            id: detailed
        }
    }
}

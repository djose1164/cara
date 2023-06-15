import QtQuick
import QtQuick.Controls
import "qrc:/product"
import "qrc:/utils"

Rectangle {
    id: root
    property string name
    property alias detailed: detailed.text
    property alias source: productImage.source

    width: 200
    height: 100

    BusyStackLayout {
        id: busyStackLayout
        width: 100
        height: 80
        anchors.left: root.left
        anchors.leftMargin: 5
        anchors.verticalCenter: root.verticalCenter

        ProductImage {
            id: productImage
            onReady:  busyStackLayout.currentIndex = 1
            sourceSize: "100x100"
        }
    }

    Column {
        anchors.left: busyStackLayout.right
        anchors.leftMargin: 10

        Label {
            text: name
            font.bold: true
        }

        Label {
            id: detailed
        }
    }
}

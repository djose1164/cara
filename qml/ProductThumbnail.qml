import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/product"
import "qrc:/utils"

BusyStackLayout {
    id: root
    property string name
    property alias detailed: detailed.text
    property alias source: productImage.source

    width: 200
    height: 120

    GridLayout {
        id: row
        columns: 2

        ProductImage {
            id: productImage
            onReady:  root.currentIndex = 1
            Layout.rowSpan: 2
            sourceSize: "100x100"
        }

        Label {
            Layout.fillWidth: true
            text: name
            font.bold: true
            wrapMode: Label.Wrap
        }

        Label {
            id: detailed
            Layout.fillWidth: true
            wrapMode: Label.Wrap
            padding: 6
        }
    }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"

Rectangle {
    id: root
    property string name
    property alias detailed: detailed.text
    property alias source: productImage.source

    BusyStackLayout {
        id: stackLayout
        anchors.fill: parent

        GridLayout {
            id: row
            columns: 2
            rows: 2

            Item {
                Layout.fillHeight: true
                Layout.rowSpan: 2
                Layout.preferredWidth: 100
                Layout.preferredHeight: 100

                ProductImage {
                    id: productImage
                    sourceSize: "100x100"

                    onReady: stackLayout.currentIndex = 1
                }
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
            }
        }
    }
}

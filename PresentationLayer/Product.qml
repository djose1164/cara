import QtQuick
import QtQuick.Controls

Rectangle {
    id: root

    property alias imgSrc: img.source
    property alias nameText: nameField.text
    property alias inStock: stocks.inStock
    property alias price: sellPriceField.text
    property alias description: descriptionField.text
    property bool canModify

    signal imageReady
    signal updateStocksClicked

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
            enabled: canModify
        }

        ProductStocks {
            id: stocks

            onUpdateClicked: updateStocksClicked()

            width: root.width
            anchors.left: parent.left
            anchors.leftMargin: 10
            canUpdate: canModify
        }

        Row {
            anchors.left: parent.left
            anchors.leftMargin: 10
            Label {
                text: `Precio: RD$`
                font.pointSize: 12
            }

            TextEdit {
                id: sellPriceField
                enabled: canModify
            }
        }

        TextEdit {
            id: descriptionField
            anchors.left: parent.left
            anchors.leftMargin: 10
            enabled: userIsAdmin
            visible: length
        }
    }
}

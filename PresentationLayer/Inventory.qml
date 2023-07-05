import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"
import "qrc:/product"

Page {
    id: root

    readonly property StackView stackView: StackView.view

    title: "Inventario"
    header: TitleHeader {
        title: root.title
        stackView: root.stackView
    }

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        anchors.topMargin: 5

        // FIXME: A new component `InventoryView` must be created.
        ProductView {
            id: productView

            onEntriesAdded: total => { if (total === 0) stackLayout.currentIndex = 1 }

            delegate: ItemDelegate {
                id: item
                required property int id
                required property var product
                required property var stocks

                onClicked: stackView.push("qrc:/product/ProductForm.qml", {inventoryId: id, product, stocks, mode: "edit"})
                Component.onCompleted: print("name: ", product.name)

                width: root.width
                height: childrenRect.height

                ProductThumbnail {
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    anchors.right: parent.right
                    anchors.rightMargin: 5

                    width: parent.width
                    height: 75
                    detailed: `Disponible ${stocks.in_stock}`
                    source: item.product.image_url
                    name: item.product.name
                }
            }
        }
        Item {
        Label {
            id: label
            anchors.centerIn: parent
            width: parent.width*.9
            horizontalAlignment: Label.AlignHCenter
            text: "Tu inventario esta vacio. Intenta crear una nueva [Orden de Compra](qrc:/orders/BuyOrder.qml)."
            onLinkActivated: link => stackView.push(link)
            textFormat: Label.MarkdownText
            font.pointSize: 14
            wrapMode: Label.WordWrap
        }
        }
    }

    RoundButton {
        onClicked: stackView.push("qrc:/product/ProductForm.qml")
        anchors {
            bottom: parent.bottom
            bottomMargin: 15
            right: parent.right
            rightMargin: 10
        }
        Material.background: Material.Blue
        Material.foreground: "white"
        icon.source: "qrc:/icons/new_product.svg"
        icon.width: 24
        icon.height: 24
    }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Utils
import Product

Page {
    id: root

    required property int salespersonId
    readonly property StackView stackView: StackView.view

    title: "Inventario"
    header: TitleHeader {
        title: root.title
        onBackClicked: stackView.pop()
    }

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        anchors.topMargin: 5

        // FIXME: A new component `InventoryView` must be created.
        ProductView {
            id: productView
            salespersonId: root.salespersonId
            onEntriesAdded: total => { if (total === 0) stackLayout.currentIndex = 1 }

            delegate: ItemDelegate {
                id: item
                required property int id
                required property var product
                required property var warehouse
                required property int quantity_available
                required property int minimum_stock_level
                required property int maximum_stock_level
                required property int reorder_point

                onClicked: {
                    stackView.push(
                        "qrc:/qt/qml/Inventory/InventoryForm.qml",
                        {
                            inventoryId: id,
                            product,
                            warehouse,
                            minimunStockLevel: minimum_stock_level,
                            maximunStockLevel: maximum_stock_level,
                            reorderPoint: reorder_point,
                            quantityAvailable: quantity_available
                        }
                    );
                }
                width: root.width
                height: 100

                ProductThumbnail {
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    anchors.right: parent.right
                    anchors.rightMargin: 5

                    width: parent.width
                    height: 100
                    detailed: `Disponible ${quantity_available}`
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
}

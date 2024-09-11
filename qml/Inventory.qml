import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import Cara.Controls
import Cara.Product
import Cara.Network

Page {
    id: root

    required property int salespersonId
    readonly property T.StackView stackView: StackView.view

    title: "Inventario"
    header: TitleHeader {
        title: root.title
        btnLoader.sourceComponent: ToolButton {
            icon.source: "qrc:/icons/complete_order.svg"
            onClicked: root.stackView.pushItem("qrc:/orders/BuyOrder.qml")
            ToolTip.text: "Ordenes de Compra"
            ToolTip.visible: hovered
            ToolTip.delay: 1000
            ToolTip.timeout: 5000
        }

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
                readonly property bool hasImages: item.product.images && item.product.images.length

                onClicked: {
                    item.product.image_url = thumbnail.source;
                    stackView.push(
                        "qrc:/qt/qml/Cara/Inventory/InventoryForm.qml",
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
                    id: thumbnail
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    anchors.right: parent.right
                    anchors.rightMargin: 5

                    width: parent.width
                    height: 100
                    detailed: `Disponible ${quantity_available}`
                    source: item.hasImages ? Api.uploadsPath + item.product.images[0].image_url : "qrc:/cara-logo-v1.png" //root.images && root.images.length ? Constants.uploadsPath + images[0].image_url : "qrc:/cara-logo-v1.png"
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

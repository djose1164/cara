pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import Cara.Controls
import Cara.ProductInventory
import Cara.Salesperson

Page {
    id: root

    property int salespersonId
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

        onBackClicked: root.stackView.popCurrentItem()
    }

    Component.onCompleted: {
        if (internal.isCurrentSalesperson)
            return;

        productModel.update();
    }

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        anchors.topMargin: 5
        currentIndex: internal.model.count < 1 ? 1 : 0

        // FIXME: A new component `InventoryView` must be created.
        ProductView {
            id: productView
            salespersonId: root.salespersonId
            listView.model: internal.model
            delegate: ItemDelegate {
                id: item
                required property var model
                required property int quantityAvailable
                required property int minimunStockLevel
                required property int maximunStockLevel
                required property int reorderPoint
                readonly property bool hasImages: model.images && model.images.length

                onClicked: {
                    stackView.push(
                        "qrc:/qt/qml/Cara/ProductInventory/InventoryForm.qml",
                        {
                            model,
                            minimunStockLevel,
                            maximunStockLevel,
                            reorderPoint,
                            quantityAvailable,
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
                    detailed: `Disponible ${item.quantityAvailable}`
                    source: item.model.imageUrl
                    name: item.model.productName
                }
            }

            BusyIndicator {
                id: busyIndicator
                anchors.centerIn: parent
                visible: productView.listView.isEmpty && productModel.status === ProductModel.Busy
            }
        }

        Item {
            Label {
                id: label
                anchors.centerIn: parent
                width: parent.width*.9
                horizontalAlignment: Label.AlignHCenter
                text: "Tu inventario esta vacio. Intenta crear una nueva [Orden de Compra](qrc:/orders/BuyOrder.qml)."
                onLinkActivated: link => root.stackView.pushItem(link)
                textFormat: Label.MarkdownText
                font.pointSize: 14
                wrapMode: Label.WordWrap
            }
        }
    }

    ProductModel {
        id: productModel
        repository: SellerProductRepositoryREST {
            sellerId: root.salespersonId
        }

        //onUpdated:
    }

    QtObject {
        id: internal
        readonly property bool isCurrentSalesperson: root.salespersonId === SalespersonSettings.salespersonId
        readonly property ProductModel model: isCurrentSalesperson ? ProductInventoryController
                                                                        .inventory
                                                                   : productModel
    }
}

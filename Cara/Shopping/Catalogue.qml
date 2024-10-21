import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import Cara.Shopping
import Cara.Controls
import Cara.Product
import Cara.ProductInventory

Page {
    id: root

    readonly property T.StackView stackView: StackView.view
    readonly property ProductModel productModel: ProductInventoryController.catalog

    title: "Catalogo"
    header: TitleHeader {
        title: root.title
        onBackClicked: root.stackView.popCurrentItem()
    }

    Component.onCompleted: {
        if (productModel.count === 0)
            catalogueRepository.fetchProducts();
    }

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        anchors.topMargin: 5

        // FIXME: A new component `InventoryView` must be created.
        ListView {
            id: productView
            model: root.productModel
            delegate: ItemDelegate {
                id: item
                required property int productId
                required property int categoryId
                required property var images
                required property string description
                required property string productName
                required property int price
                required property url imageUrl
                required property var model

                width: root.width
                height: 100

                onClicked: {
                    const path = "qrc:/qt/qml/Cara/Product/EditProductPage.qml";
                    root.stackView.pushItem(path, {model});
                    const currentItem = (root.stackView.currentItem as ProductForm);
                    currentItem.refreshRequested.connect(productId => {
                        console.log("Handling editRefreshRequested in Catalogue. productId: "+productId);
                        root.productModel.refreshBuyPriceHistory(productId);
                    });
                }

                ProductThumbnail {
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    anchors.right: parent.right
                    anchors.rightMargin: 5

                    width: parent.width
                    height: 100
                    source: item.imageUrl.toString() ? item.imageUrl : "qrc:/cara-logo-v1.png"
                    name: item.productName
                    detailed: item.description
                }
            }

            BusyIndicator {
                anchors.centerIn: parent
                running: productView.count === 0
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

    CatalogueRepository {
        id: catalogueRepository
        onAllProductsFetched: products => root.productModel.updateFromList(products)
    }

    // ProductModel {
    //     id: productModel
    // }

    RoundButton {
        onClicked: root.stackView.pushItem("qrc:/qt/qml/Cara/Product/AddProductPage.qml", {productModel})
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

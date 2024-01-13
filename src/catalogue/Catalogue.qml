import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import catalogue as Catalogue
import "qrc:/utils"
import "qrc:/product"
import "qrc:/singleton"

Page {
    id: root

    readonly property StackView stackView: StackView.view

    title: "Catalogo"
    header: TitleHeader {
        title: root.title
        onBackClicked: stackView.pop()
    }

    Component.onCompleted: catalogueRepository.fetchProducts()

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        anchors.topMargin: 5

        // FIXME: A new component `InventoryView` must be created.
        ListView {
            id: productView

            model: listModel

            delegate: ItemDelegate {
                id: item
                required property int id
                required property int category_id
                required property url image_url
                required property string description
                required property string name
                required property int sell_price
                required property int buy_price

                onClicked: {
                    stackView.push("qrc:/product/ProductForm.qml", {
                        productId: id,
                        name,
                        imageUrl: image_url,
                        categoryId: category_id,
                        description,
                        buyPrice: buy_price,
                        sellPrice: sell_price,
                        mode: "edit"
                   });
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
                    source: item.image_url
                    name: item.name
                    detailed: description
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

    ListModel { id: listModel }

    Catalogue.CatalogueRepository {
        id: catalogueRepository
        onProductsFetched: data => listModel.append(JSON.parse(data).products)
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

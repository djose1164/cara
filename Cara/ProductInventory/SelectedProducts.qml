pragma ComponentBehavior: Bound

import QtQuick
import Cara.Controls
import Cara.Product
import Cara.ProductInventory

Item {
    id: root

    property alias model: productModel
    property alias delegate: listView.delegate
    readonly property alias implicitViewWidth: listView.width
    readonly property alias implicitViewHeight: listView.height

    signal redirectClicked
    signal deleteClicked(index: int, productId: int)

    RedirectGroupBox {
        id: groupBox
        anchors.fill: parent
        title: "Producto(s) agregados"
        onRedirectClicked: root.redirectClicked()

        ListView {
            id: listView
            anchors.fill: parent
            spacing: 5
            clip: true
            model: productModel
            delegate: ProductCard {
                id: productCard
                required productName
                required productId
                required imageUrl
                required index
                required property int price
                required property var model

                width: listView.width
                productPrice: price
                total: price * model.selectedQty
                spinBox.to: model.quantityAvailable
                spinBox.onValueChanged: productCard.model.selectedQty = spinBox.value
                //onDeleted: deleteClicked(index, productId)
            }

            Connections {
                target: listView.currentItem

                function onDeleted(index: int, productId: int) {
                    root.deleteClicked(index, productId);
                }
            }
        }
    }

    ProductModel { id: productModel }

    function toJson() {
        const products = [];
        for (let i = 0; i < model.count; ++i) {
            const product = model.get(i);
            products.push({product_id: product.productId, quantity: product.selectedQty});
        }
        return products;
    }
}

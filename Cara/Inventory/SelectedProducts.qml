import QtQuick
import QtQuick.Controls
import Cara.Utils
import Cara.Product
import Cara.Inventory

Item {
    id: root

    property alias model: productModel
    property alias delegate: listView.delegate

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
                required property int price
                required property var model
                required property int index

                width: listView.width
                productPrice: price
                total: price * model.selectedQty
                spinBox.onValueChanged: productCard.model.selectedQty = spinBox.value
                onDeleted: deleteClicked(index, productId)
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

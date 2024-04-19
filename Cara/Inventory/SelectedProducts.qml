import QtQuick
import QtQuick.Controls
import Cara.Utils
import Cara.Product
import Cara.Inventory

Item {
    id: root

    property alias model: productModel

    signal redirectClicked
    signal deleteClicked(index: int, productId: int)

    RedirectGroupBox {
        id: groupBox
        anchors.fill: parent
        title: "Producto(s) agregados"
        redirectText: "Editar producto(s)"
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
                required property int sellPrice
                required property var model
                required property int index

                width: listView.width
                productPrice: sellPrice
                total: sellPrice * model.selectedQty
                spinBox.onValueChanged: productCard.model.selectedQty = spinBox.value
                onDeleted: deleteClicked(index, productId)
                Component.onCompleted: console.log(`SelectedProduct: total: ${sellPrice}x${productCard.model.selectedQty}`)
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

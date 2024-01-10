import QtQuick
import QtQuick.Controls
import Utils
import "qrc:/product"

Item {
    id: root

    property alias model: listView.model

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
            delegate: ProductCard {
                required productName
                required productId
                required imageUrl
                required property int sellPrice
                required property var model
                required property int index

                width: listView.width
                productPrice: sellPrice
                total: sellPrice * model.selectedQty
                spinBox.onValueChanged: model.selectedQty = spinBox.value
                onDeleted: deleteClicked(index, productId)
            }
        }
    }

    function toJson() {
        const products = [];
        for (let i = 0; i < model.count; ++i) {
            const product = model.get(i);
            products.push({product_id: product.productId, quantity: product.selectedQty});
        }
        return products;
    }
}

pragma ComponentBehavior: Bound

import QtQuick
import Cara.Controls
import Cara.ProductInventory

Item {
    id: root

    property alias delegate: listView.delegate
    property alias model: listView.model
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
                onDeleted: root.deleteClicked(model.index, productCard.productId)
            }
        }
    }
}

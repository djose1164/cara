import QtQuick
import Cara.Product

ProductCard {
    id: root

    required property var model
    required index
    required productName
    required productId
    required imageUrl
    required index

    implicitWidth: 120
    implicitHeight: 90
    total: internal.profitTotal
    productPrice: model.price
    spinBox.onValueChanged: root.model.selectedQty = spinBox.value

    QtObject {
        id: internal
        readonly property double profitPerUnit: root.model.price - root.model.supplierPrice
        readonly property double profitTotal: profitPerUnit * root.model.selectedQty
        readonly property double imgSize: Math.min(root.width/2, root.height/2)
    }
}

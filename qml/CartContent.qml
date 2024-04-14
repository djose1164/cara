import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Product
import Shopping
import data

Item {
    id: root

    readonly property alias count: listView.count

    implicitWidth: listView.implicitWidth
    implicitHeight: listView.implicitHeight

    ListView {
        id: listView
        anchors.fill: parent
        anchors.topMargin: 5
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        spacing: 5
        model: CartController.model
        delegate: ProductCard {
            required property var model
            required property product product
            required property int quantity
            required total
            required property int index

            width: listView.width
            productName: product.name
            productPrice: product.price
            imageUrl: product.imageUrl
            productId: product.productId
            spinBox.value: quantity

            onDeleted: CartController.removeFromCart(index)
            spinBox.onValueChanged: model.quantity = spinBox.value
        }
    }
}

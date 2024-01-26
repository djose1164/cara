import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import core
import entities as Entity
import Product
import "qrc:/singleton"

ListView {
    id: root

    spacing: 5
    model: CartBusiness.model
    delegate: ProductCard {
        required property var model
        required property Entity.Product product
        required property int quantity
        required total
        required property int index

        onDeleted: CartBusiness.removeFromCart(index)
        spinBox.onValueChanged: model.quantity = spinBox.value

        productName: product.name
        productPrice: product.price
        imageUrl: product.imageUrl
        productId: product.productId
    }
}

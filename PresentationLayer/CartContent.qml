import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.cara 1.0
import "qrc:/product"
import "qrc:/singleton"

ListView {
    id: root

    model: CartBusiness.model
    delegate: ProductCard {
        required property var model
        required property ProductEntity product
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

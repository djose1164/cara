import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/product"
import "qrc:/singleton"

ListView {
    id: listView

    model: TheCart.model
    delegate: ProductCard {
        id: root
        required imageUrl
        required productId
        required property string name
        required property int price
        required property int quantity
        required property int index

        onDeleted: TheCart.removeFromCart(index)
        spinBox.onValueChanged: listView.model.set(index, {quantity: spinBox.value})

        productName: name
        productPrice: price

    }
}

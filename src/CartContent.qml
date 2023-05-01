import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/product"
import "qrc:/singleton"

ListView {
    id: listView

    clip: true
    model: TheCart.model
    delegate: ProductCard {
        id: root
        required property string image_url
        required property string name
        required property int sell_price
        required property int quantity
        required property int id
        required property int index

        onDeleted: TheCart.removeFromCart(index)

        imageUrl: image_url
        productName: name
        productPrice: sell_price
        productId: id
        productQuantity: quantity
    }
}

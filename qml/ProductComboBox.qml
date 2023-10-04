import QtQuick
import QtQuick.Controls
import "qrc:/singleton"

ComboBox {
    id: productField

    Component.onCompleted: if (ProductManager.modelIsEmpty()) productRequest.addSuggetionsToModel()

    model: ProductManager.model
    textRole: "name"
    valueRole: "sell_price"
    currentIndex: -1
    displayText: currentIndex === -1 ? "Seleccionar Producto" : currentText

    ProductRequest {
        id: productRequest
        onSucceeded: products => ProductManager.modelAppend(products)
    }
}

import QtQuick
import QtQuick.Controls
import "qrc:/singleton"

ComboBox {
    id: root

    enum ValueRole {
        SellPrice,
        BuyPrice
    }

    property int valueRoleType: ProductComboBox.SellPrice
    Component.onCompleted: if (ProductManager.modelIsEmpty()) productRequest.addSuggetionsToModel()

    model: ProductManager.model
    textRole: "name"
    valueRole: getValueRole()
    currentIndex: -1
    displayText: currentIndex === -1 ? "Seleccionar Producto" : currentText

    ProductRequest {
        id: productRequest
        onSucceeded: products => products.forEach(e => ProductManager.modelAppend(e))
    }

    function getValueRole(): string {
        switch (valueRoleType) {
        case ProductComboBox.SellPrice:
            return "sell_price";
        case ProductComboBox.BuyPrice:
            return "buy_price";
        default:
            return "buy_price";
        }
    }
}

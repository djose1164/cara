import QtQuick
import QtQuick.Controls
import Cara.ProductInventory

ComboBox {
    id: root

    readonly property bool selected: currentIndex !== -1

    model:  ProductInventoryController.supplierModel
    textRole: "name"
    valueRole: "supplierId"
    displayText: currentIndex === -1 ? "Seleccionar suplidor" : currentText

    function toJson() {
        return {
            supplierId: root.currentValue
        };
    }
}

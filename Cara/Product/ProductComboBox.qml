import QtQuick
import QtQuick.Controls

ComboBox {
    id: root
    editable: true
    textRole: "productName"
    valueRole: "buyPrice"
    currentIndex: editable ? 0 : -1
    displayText: currentIndex === -1 && !editable ? "Seleccionar Producto" : currentText
    onCurrentIndexChanged: console.log("ProductComboBox.currentValue:",currentValue)
}

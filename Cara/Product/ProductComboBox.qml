import QtQuick
import QtQuick.Controls

ComboBox {
    id: root

    textRole: "name"
    valueRole: "price"
    currentIndex: -1
    displayText: currentIndex === -1 ? "Seleccionar Producto" : currentText
}

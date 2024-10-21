import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cara.Product

RowLayout {
    id: root

    required property int index
    required property var productModel

    readonly property int quantity: quantityField.value
    readonly property alias price: productField.currentValue
    readonly property var entryValue: {
        "product_id": root.productModel.get(productField.currentIndex).productId,
        "quantity": quantity,
        "unit_price": parseFloat(price),
    }
    readonly property int total: price * quantity
    readonly property var model: productField.model

    signal created(entry: var)
    signal updated(idx: int, entry: var)
    signal deleted(idx: int)

    onEntryValueChanged: updated(index, entryValue)

    width: ListView.view.width

    ProductComboBox {
        id: productField
        model: root.productModel
        Layout.fillWidth: true
        Layout.preferredHeight: 50
    }

    SpinBox {
        id: quantityField
        Layout.preferredHeight: productField.height*.8
        Layout.preferredWidth: root.width*.25
        Layout.alignment: Qt.AlignCenter
        from: 1
        editable: true
        Keys.onReturnPressed: Qt.inputMethod.hide()
    }

    RoundButton {
        onClicked: deleted(index);
        Image {
            anchors.centerIn: parent;
            source: "qrc:/icons/trash-can-outline.svg"
        }
        flat: true
    }
}

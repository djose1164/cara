import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    id: root
    readonly property int quantity: quantityField.value
    readonly property alias price: productField.currentValue
    readonly property var entryValue: {
        "product_id": findProductId(productField.currentText),
        "quantity": quantity,
        "unit_price": price
    }
    readonly property int total: price * quantity
    required property int index
    readonly property ListModel productsModel: ListView.view.productsModel

    signal created(entry: var)
    signal updated(idx: int, entry: var)
    signal deleted(idx: int)

    onEntryValueChanged: if (entryValue?.unit_price) updated(index, entryValue)

    width: ListView.view.width

    ComboBox {
        id: productField
        Layout.fillWidth: true
        model: root.productsModel
        textRole: "name"
        valueRole: "sell_price"
        currentIndex: -1
        displayText: currentIndex === -1 ? "Seleccionar Producto" : currentText
    }

    SpinBox {
        id: quantityField
        Layout.preferredHeight: productField.height
        Layout.maximumWidth: root.width*.3
        Layout.minimumWidth: 100
        from: 1
        editable: true
        Layout.preferredWidth: root.width*.4
        Keys.onReturnPressed: Qt.inputMethod.hide()
    }

    RoundButton {
        onClicked: deleted(index);
        IconImage {
            anchors.centerIn: parent;
            source: "qrc:/icons/trash-can-outline.svg"
        }
        flat: true
        Layout.alignment: Qt.AlignTrailing
    }

    function find(criteria) {
        for (let i = 0; i < productsModel.count; ++i)
            if (criteria(productsModel.get(i)))
                return productsModel.get(i);
        return null;
    }

    function findProductId(productName) {
        const pred = item => item.name === productName;
        const fetched = find(pred);
        if (!fetched)
            return null;
        return fetched.id;
    }
}

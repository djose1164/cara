import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Product

RowLayout {
    id: root

    required property int index

    readonly property int quantity: quantityField.value
    readonly property alias price: productField.currentValue
    readonly property var entryValue: {
        "product_id": findProductId(productField.currentText),
        "quantity": quantity,
        "unit_price": price,
    }
    readonly property int total: price * quantity
    readonly property ListModel model: productField.model

    property alias valueRoleType: productField.valueRoleType

    signal created(entry: var)
    signal updated(idx: int, entry: var)
    signal deleted(idx: int)

    onEntryValueChanged: updated(index, entryValue)

    width: ListView.view.width

    ProductComboBox {
        id: productField
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
        IconImage {
            anchors.centerIn: parent;
            source: "qrc:/icons/trash-can-outline.svg"
        }
        flat: true
    }

    function find(criteria) {
        for (let i = 0; i < model.count; ++i)
            if (criteria(model.get(i)))
                return model.get(i);
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

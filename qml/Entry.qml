import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/product"
import core

RowLayout {
    id: root
    readonly property int quantity: quantityField.value
    readonly property alias price: productField.currentValue
    readonly property var entryValue: {
        "product_id": findProductId(productField.currentText),
        "quantity": quantity,
        "unit_price": price,
        "warehouse_id": CurrentUser.user.warehouseId
    }
    readonly property int total: price * quantity
    required property int index
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
    }

    SpinBox {
        id: quantityField
        Layout.preferredHeight: productField.height*.8
        Layout.preferredWidth: root.width/3
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
        Layout.alignment: Qt.AlignTrailing
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

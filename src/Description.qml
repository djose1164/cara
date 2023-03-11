import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/customers"
import "qrc:/product"

Item {
    id: root
    property int payment: 2
    property int _index: 0
    property var details
    property int total: 0
    property int amountPaid: amountField.text ? parseInt(amountField.text) : 0

    signal okClicked
    signal updateTotal

    Component.onCompleted: payment = 0
    onOkClicked: productEntry.okClicked()
    onPaymentChanged: {
        if (payment !== 2)
            amountField.clear();

        switch (payment){
        case 0:
            amountField.placeholderText = "Monto a pagar: " + total;
            amountPaid = amountField.text ? parseInt(amountField.text) : 0;
            break;
        case 1:
            amountField.placeholderText = "Credito: " + total;
            break;
        case 2:
            amountField.placeholderText = "Ingrese monto";
            break;
        }
    }

    onUpdateTotal: {
        total = 0;
        for (let i = 0; i < orderModel.count; ++i) {
            let current = orderModel.get(i).entryValue;
            total += current.quantity * current.unit_price;
        }

        if (payment === 0)
            amountField.placeholderText = "Monto a pagar: " + total;
        else if (payment === 1)
            amountField.placeholderText = "Credito: " + total;
    }

    ProductEntry {
        id: productEntry
        width: parent.width
        height: parent.height * .35
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 20
        Component.onCompleted: updateTotal.connect(root.updateTotal)
    }

    RowLayout {
        width: parent.width
        anchors.top: productEntry.bottom
        anchors.topMargin: 50
        spacing: 10

        Button {
            text: "Agregar"
            onClicked: repeaterModel.append({"currentEntryIndex": _index++})
            Layout.fillWidth: true
        }

        Button {
            text: "Remover"
            onClicked: if (_index > 1) removeEntryAt(--_index)
            Layout.fillWidth: true
        }
    }

    ListModel {
        id: repeaterModel
        Component.onCompleted: repeaterModel.append({"currentEntryIndex": _index++})
    }

    function findIn(model, criteria) {
        for (let i = 0; i < model.count; ++i)
            if (criteria(model.get(i)))
                return model.get(i);
        return null;
    }

    // Reset entires to one
    function resetEntries() {
        for (let i = _index; i != 0; --i)
            removeEntryAt(i);
    }

    function removeEntryAt(idx) {
        repeaterModel.remove(idx);
    }

    function clear() {
        amountPaid = 0;
        payment = 0;
        total = 0;
        amountField.clear();
    }
}

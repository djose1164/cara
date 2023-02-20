import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/client"
import "qrc:/payment" as Payments

GridLayout {
    property alias clientID: clientField.currentValue
    property alias date: dateField.text
    property int payment: 0
    property int _index: 0
    property var details
    property int total: 0
    property int amountPaid: amountField.text ? parseInt(amountField.text) : 0

    signal okClicked
    signal updateTotal
    signal updateClientField

    id: root
    columns: 2
    rowSpacing: 20

    ClientComboBox {
        id: clientField
        Layout.columnSpan: 2
        Layout.fillWidth: true
        Layout.topMargin: height/2

        Component.onCompleted: root.updateClientField.connect(clientField.updateClientField)
    }

    ScrollView {
        Layout.columnSpan: 2
        Layout.fillHeight: true
        Layout.alignment: Qt.AlignCenter
        Layout.margins: 5
        Layout.topMargin: height/2

        ColumnLayout {
            anchors.fill: parent

            Repeater {
                id: repeater
                model: repeaterModel

                Entry {
                    id: entry
                    Component.onCompleted: {
                        root.okClicked.connect(entry.okClicked)
                        entryAdded.connect(root.updateTotal)
                    }
                }
            }
        }
    }

    RowLayout {
        Layout.columnSpan: 2
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignCenter
        Layout.leftMargin: 20
        Layout.rightMargin: 20
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

    Payments.PaymentStatus {
        Layout.columnSpan: 2
        Layout.alignment: Qt.AlignCenter
        paymentState: payment
    }

    onPaymentChanged: {
        if (payment !== 2)
            amountField.clear();

        switch (payment){
        case 0:
            amountField.placeholderText = "Monto a pagar: " + total;
            amountPaid = parseInt(amountField.text);
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


    TextField {
        id: amountField
        placeholderText: "Monto a pagar: "
        enabled: payment == 2
        inputMethodHints: Qt.ImhDigitsOnly
        Layout.columnSpan: amountLabel.visible ? 1 : 2
        Layout.fillWidth: true
    }

    Label {
        id: amountLabel
        visible: payment === 2
        text: "Debera " + (total - amountPaid)
        Layout.fillWidth: true
        Layout.leftMargin: 20
    }

    TextField {
        id: dateField
        Layout.columnSpan: 2
        Layout.fillWidth: true
        placeholderText: "dia/mes/año"
        validator: RegularExpressionValidator {
            regularExpression: /^([0-2][0-9]|(3)[0-1])(\/)(((0)[0-9])|((1)[0-2]))(\/)\d{4}$/
        }
        inputMethodHints: Qt.ImhNoPredictiveText
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
    }
}

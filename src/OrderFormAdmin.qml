import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/payment" as Payments

AddOrder {
    id: root
    ClientComboBox {
        id: clientField
        Layout.fillWidth: true
        Layout.topMargin: height/2
        Component.onCompleted: root.updateClientField.connect(clientField.updateClientField)
    }

    RowLayout {
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

    Payments.PaymentStatus {
        Layout.alignment: Qt.AlignCenter
        group.onClicked: btn => payment = statusToInt(btn.text)
    }

    RowLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true
        TextField {
            id: amountField
            placeholderText: "Monto a pagar: "
            enabled: root.payment === 2
            inputMethodHints: Qt.ImhDigitsOnly
            Layout.alignment: Qt.AlignLeading
            Layout.fillWidth: true
        }

        Label {
            id: amountLabel
            visible: payment === 2
            text: "Debera " + (total - amountPaid)
            Layout.alignment: Qt.AlignTrailing
            Layout.fillWidth: true
        }
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
}

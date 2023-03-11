import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/payment" as Payments
import "qrc:/customers"

Page {
    id: root

    header: AddOrderHeader {}

    ColumnLayout {
        anchors.fill: parent
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        spacing: 20

        ClientComboBox {
            id: clientField
            Layout.fillWidth: true
            Component.onCompleted: root.updateClientField.connect(clientField.updateClientField)
        }

        AddOrder {
            id: addOrder
            Layout.fillWidth: true
            Layout.fillHeight: true
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

        Button {
            id: okBtn
            text: "Ok"
            onClicked: okHandler()
            Layout.fillWidth: true
        }
    }
}

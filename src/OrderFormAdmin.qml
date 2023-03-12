import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/payment" as Payments
import "qrc:/customers"

Page {
    id: root
    readonly property bool isACreditOrder: paymentStatus.checkedButton.text === "Credito"
    readonly property bool isCredited: paymentStatus.checkedButton.text === "Abono"
    readonly property bool isPaid: paymentStatus.checkedButton.text === "Pago"
    readonly property int amount: amountField.text ? amountField.text : 0
    readonly property int total: amountField.text ? addOrder.totalToPay - amount : addOrder.totalToPay

    header: AddOrderHeader {}

    ColumnLayout {
        anchors.fill: parent
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        spacing: 20

        ClientComboBox {
            id: clientField
            Layout.fillWidth: true
        }

        AddOrder {
            id: addOrder
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Payments.PaymentStatus {
            id: paymentStatus
            Layout.alignment: Qt.AlignCenter
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            TextField {
                id: amountField
                placeholderText: isPaid || isACreditOrder ? `Monto a pagar: ${total}`
                                                      : "Ingrese monto..."
                enabled: isCredited
                inputMethodHints: Qt.ImhDigitsOnly
                Layout.alignment: Qt.AlignLeading
                Layout.fillWidth: true
            }

            Label {
                id: amountLabel
                visible: isCredited
                text: `Debera: RD$${total}`
                font.pointSize: 12
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

    function okHandler() {
        addOrder.stackLayout.currentIndex = 0;
        const details = [];
        for (let i = 0; i < addOrder.orderModel.count; ++i)
            details.push(addOrder.orderModel.get(i).entryValue);

        const fullOrder = {
            customer_id: clientField.currentValue,
            date: dateField.text ? dateField.text : new Date().toLocaleDateString(
                                         Qt.locale(), "dd/MM/yyyy"),
            order_details: details,
            payment: {
                amount_to_pay: addOrder.totalToPay,
                paid_amount: amount
            }
        };
        print('fullOrder ', JSON.stringify(fullOrder, null, 2));
        addOrder.request.post("/orders/", fullOrder);
    }
}

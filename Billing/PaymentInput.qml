import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Billing
import Product
import Utils

Item {
    id: root

    required property CostCalculator calculator

    property alias checkState: paymentStatus.checkState
    readonly property bool isACreditOrder: paymentStatus.buttonText === "Credito"
    readonly property bool isPartiallyPaid: paymentStatus.buttonText === "Abono"
    readonly property bool isPaid: paymentStatus.buttonText === "Pago"
    readonly property alias text: amountField.text
    readonly property int value: parseInt(text)
    readonly property alias selectedStatus: paymentStatus.selectedStatus

    width: 200
    implicitHeight: column.childrenRect.height

    states: [
        State {
            name: "inputDisabled"
            when: paymentStatus.selectedStatus === Payment.Paid
                  || paymentStatus.selectedStatus === Payment.Unpaid
            PropertyChanges {
                target: amountField
                text: ""
            }
        }
    ]

    Column {
        id: column
        spacing: 20

        PaymentStatus {
            id: paymentStatus
            width: root.width
            height: 75
            onSelectedStatusChanged: amountField.text = ""
        }

        RowLayout {
            height: 50
            width: root.width
            NumberTextField {
                id: amountField
                placeholderText: getPlaceholderText()
                enabled: paymentStatus.selectedStatus === Payment.Credited
                         && calculator.total > 0
                inputMethodHints: Qt.ImhDigitsOnly
                intValidator.top: calculator.total
                Layout.alignment: Qt.AlignLeading
                Layout.fillWidth: true
            }

            Label {
                id: amountLabel
                visible: isPartiallyPaid && calculator.total > 0
                text: {
                    const debt = calculator.total - value;
                    if (debt > 0)
                        return `Debera: RD$${debt}`;
                    else
                        "Error: Monto invalido";
                }
                font.pointSize: 12
            }
        }
    }

    QtObject {
        id: internal
        readonly property int debt: calculator.total - value
    }

    function getPlaceholderText() : string {
        const total = calculator.total;
        if (total === 0)
            return "La orden debe contener al menos un producto";
        if (isPaid || isACreditOrder)
            return `Monto a pagar: ${total}`;
        else if (!paymentStatus.checkedButton)
            return "Seleccione un estado";
        else
            return "Ingrese monto...";
    }

    function toJson() {
        const isPaid = selectedStatus === Payment.Paid;
        const calcTotal = calculator.total;
        const paidAmount = isPaid ? calcTotal : value;
        const payment = {
            paid_amount: paidAmount,
            amount_to_pay: calcTotal,
            payment_status_id: selectedStatus
        };
        if (selectedStatus !== Payment.Unpaid)
            payment.payment_details = [{amount: paidAmount}];
        return payment;
    }

    function validate() {
        if (checkState === Qt.Unchecked)
            throw "Debes selecionar la condicion de pago de la orden";
        else if(selectedStatus === Payment.Credited && root.value < 1)
            throw "Debes digitar un monto valido mayor a 0";
        else if (selectedStatus === Payment.Credited && internal.debt < 0)
            throw "El monto digitado excede el total de la orden.";
    }
}

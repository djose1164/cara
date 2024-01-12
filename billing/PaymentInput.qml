import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import entities
import Product
import "qrc:/utils"

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

    Column {
        id: column
        spacing: 20

        PaymentStatus {
            id: paymentStatus
            width: root.width
            height: 75
        }

        RowLayout {
            height: 50
            width: root.width
            NumberTextField {
                id: amountField
                placeholderText: getPlaceholderText()
                enabled: paymentStatus.selectedStatus === PaymentEntity.Credited
                         && calculator.total > 0
                inputMethodHints: Qt.ImhDigitsOnly

                Layout.alignment: Qt.AlignLeading
                Layout.fillWidth: true
            }

            Label {
                id: amountLabel
                visible: isPartiallyPaid && calculator.total > 0
                text: `Debera: RD$${calculator.total}`
                font.pointSize: 12
            }
        }
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
        const payment_status_id = value === calculator.total
                                ? PaymentEntity.Paid : (value === 0)
                                  ? PaymentEntity.Unpaid : PaymentEntity.Credited;
        const isPaid = selectedStatus === PaymentEntity.Paid;
        const calcTotal = calculator.total;
        return {
            paid_amount: isPaid ? calcTotal : value,
            amount_to_pay: calcTotal,
            payment_status_id
        };
    }
}

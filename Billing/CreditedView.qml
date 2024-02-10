import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Utils
import Billing

// TODO: Make two views: one for just seeing & the another for editing.
Item {
    id: root

    required property payment payment_
    readonly property bool amountIsInRangge: internal.amount > 0
                                             && internal.amount <= internal.debt

    signal creditClicked(creditedAmount: int)
    signal success(newPayment: payment)
    signal error

    implicitHeight: columnLayout.implicitHeight
    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        spacing: 10

        Label {
            text: "La deuda asciende a RD$" + internal.debt
            font.pointSize: 14
        }

        Label {
            text: "El cliente ha abonado RD$" + payment_.paidAmount
            font.pointSize: 14
        }

        TextField {
            id: newPayment
            placeholderText: "Ingrese monto a abonar"
            Layout.fillWidth: true
        }

        Button {
            text: "Abonar"
            enabled: amountIsInRangge
            Layout.fillWidth: true
            onClicked: {
                creditClicked(internal.amount);
                payService.pay(internal.amount)
            }
        }
    }

    RestService {
        Pay {
            id: payService
            paymentId: payment_.paymentId
            path: "payments"
            onPaid: resp => {
                root.success(paymentController.fromJsonObject(resp.payment));
            }
            onErrorOccurred: root.error()
        }
    }

    PaymentController {
        id: paymentController
    }

    QtObject {
        id: internal
        readonly property int amount: newPayment.text ? parseInt(newPayment.text) : 0
        readonly property int debt: payment_.amountToPay - payment_.paidAmount
    }
}

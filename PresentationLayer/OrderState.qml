import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/payment"
import "qrc:/singleton"
import com.cara 1.0
import "qrc:/product"

Item {
    id: root

    property PaymentEntity payment
    property OrderStatus status
    property date date
    property int orderId
    property OrderModel details

    readonly property var statusString: {
        0: "Pago",
        1: "Credito",
        2: "Abonado"
    }

    signal paymentApplied(newPayment: var)

    onPaymentApplied: p => payment = p

    ColumnLayout {
        anchors.fill: parent
        anchors.leftMargin: 10

        Label {
            text: `Estado de Orden ID#${orderId}`
            font.pointSize: 12.5
            font.bold: true
        }

        OrderStatusCB {
            onStatusUpdated: (newStatusId, newStatusString) => {
                status.statusId = newStatusId;
                status.statusString = newStatusString;
            }

            Layout.fillWidth: true
            orderId: root.orderId
            statusId: status.statusId
        }

        RowLayout {
            Layout.topMargin: 0
            Label { text: statusString[payment.status] }

            Button {
                text: "Actualizar"
                visible: TheCurrentUser.isAdmin
                onClicked: updatePayment.open()
            }
        }

        Label {
            text: "Fecha"
            font.pointSize: 12
            font.bold: true
        }

        Label {
            text: date.toLocaleDateString()
            font.pointSize: 12
        }

        Label {
            text: "Detalles"
            font.pointSize: 12
            font.bold: true
        }

        ScrollView {
            Layout.preferredHeight: details.count >= 2 ? 250 : 100
            Layout.fillWidth: true
        ColumnLayout {
            id: grid

            Repeater {
                id: repeater
                model: details
                delegate: ProductCard {
                    required property ProductEntity product
                    required property int quantity
                    required total

                    width: root.width-20
                    enabled: false
                    productName: product.name
                    productPrice: product.price
                    spinBox.value: quantity
                    imageUrl: product.imageUrl
                }
            }
        }
        }
    }

    UpdatePayment {
        id: updatePayment

        onPaymentPatched: newAmount => payment.paidAmount = newAmount

        amountToPay: payment.amountToPay
        paidAmount: payment.paidAmount
        paymentStatus: payment.status
        paymentId: payment.paymentId
    }
}

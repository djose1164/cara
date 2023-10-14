import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/payment"
import core 1.0
import "qrc:/product"

ColumnLayout {
    id: root

    property var payment
    property var status
    property date date
    property int orderId
    property var details

    readonly property var statusString: {
        0: "Pago",
        1: "Credito",
        2: "Abonado"
    }

    signal paymentApplied(newPayment: var)

    onPaymentApplied: p => payment = p

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
        Layout.topMargin: 20
        Layout.bottomMargin: 10

        Label {
            padding: 12
            text: statusString[payment.status]
            font.bold: true
            font.pointSize: 12
        }

        Button {
            text: "Actualizar"
            visible: CurrentUser.isAdmin
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

    ColumnLayout {
        id: productsColumn
        Layout.fillHeight: true
        Layout.fillWidth: true

        Repeater {
            id: repeater
            model: details
            delegate: ProductCard {
                required property var product
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

    UpdatePayment {
        id: updatePayment

        onPaymentPatched: newAmount => payment.paidAmount = newAmount

        amountToPay: payment.amountToPay
        paidAmount: payment.paidAmount
        paymentStatus: payment.status
        paymentId: payment.paymentId
    }
}

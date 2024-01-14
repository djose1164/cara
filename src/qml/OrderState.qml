import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Billing
import User
import Customer
import "qrc:/product"

ColumnLayout {
    id: root

    property var payment
    property var status
    property date date
    property int orderId
    property var details
    property customer buyer

    readonly property var statusString: {
        1: "Pago",
        2: "Credito",
        3: "Abonado"
    }

    signal paymentApplied(newPayment: var)

    states: [
        State {
            name: "customerView"
            when: userStore.user.isCustomer
            PropertyChanges {
                target: orderStatusCB
                enabled: false
            }
            PropertyChanges {
                target: updatePaymentBtn
                visible: false
            }
        }
    ]

    onPaymentApplied: p => payment = p

    CustomerInfoBox {
        Layout.fillWidth: true
        customer: buyer
    }

    Label {
        text: `Estado de Orden ID#${orderId}`
        font.pointSize: 12.5
        font.bold: true
    }

    OrderStatusCB {
        id: orderStatusCB
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
            id: updatePaymentBtn
            text: "Actualizar"
            onClicked: updatePayment.open()
        }
    }

    Label {
        text: "Fecha"
        font.pointSize: 12
        font.bold: true
    }

    Label {
        property date orderDate: root.date
        text: orderDate.toLocaleDateString()
        font.pointSize: 12
    }

    Label {
        text: `
            <strong>Total</strong><br>
            RD$${internal.orderTotal}
        `
        font.pointSize: 12
        Layout.topMargin: 20
    }

    Label {
        text: "Detalles"
        font.pointSize: 12
        font.bold: true
        Layout.topMargin: 20
    }

    Repeater {
        id: repeater
        model: details
        delegate: ProductCard {
            required property var product
            required property int quantity
            required total

            Component.onCompleted: internal.orderTotal += total

            Layout.fillHeight: true
            Layout.fillWidth: true
            enabled: false
            productName: product.name
            productPrice: product.price
            spinBox.value: quantity
            imageUrl: product.imageUrl
        }
    }

    LocalUserStore { id: userStore }

    UpdatePayment {
        id: updatePayment

        onPaymentPatched: newAmount => payment.paidAmount = newAmount

        amountToPay: payment.amountToPay
        paidAmount: payment.paidAmount
        paymentStatus: payment.status
        paymentId: payment.paymentId
    }

    QtObject {
        id: internal

        property int orderTotal
    }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import Billing
import User
import Customer
import Product

ColumnLayout {
    id: root

    readonly property T.StackView stackView: StackView.view
    required property var model
    property var payment: model.payment
    property var status
    property date date
    property int orderId
    property var details
    property customer buyer

    signal paymentApplied(newPayment: var)

    spacing: 20

    states: [
        State {
            name: "customerView"
            when: UserSettings.userTypeId === UserEntity.Customer
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
        Layout.fillWidth: true
        orderId: root.orderId
        statusId: status.statusId
        onStatusUpdated: (newStatusId, newStatusString) => {
            status.statusId = newStatusId;
            status.statusString = newStatusString;
        }
    }

    PaymentDetails {
        payment_: root.payment
        stackView: billStack
        Layout.bottomMargin: 10
        Layout.topMargin: 20
        Layout.fillWidth: true
        onRefresh: newPayment => model.payment = newPayment
    }

    Label {
        text: "Fecha"
        font.pointSize: 12
        font.bold: true
    }

    Label {
        property date orderDate: root.date
        text: orderDate.toLocaleDateString()
        font.pointSize: 14
    }

    Column {
        Layout.topMargin: 20
        Label {
            text: "Total"
            font.pointSize: 12
        }
        Label {
            text: `<span style="font-size: 11pt;">RD$</span><b style="font-size: 15pt;">${internal.orderTotal}</b>`
            textFormat: Label.RichText
        }
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

    Item { Layout.fillWidth: true }

    LocalUserStore { id: userStore }

    QtObject {
        id: internal

        property int orderTotal
    }
}

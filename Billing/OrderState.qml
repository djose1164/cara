import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import Billing
import User
import Customer
import Product
import data

ColumnLayout {
    id: root

    property T.StackView stackView: StackView.view
    required property var model
    property var payment: root.model.payment
    property var status
    property date date
    property int orderId
    property var details
    readonly property alias paymentDetails: paymentDetails

    spacing: 20

    states: [
        State {
            name: "customerView"
            when: UserSettings.userTypeId === UserEntity.Customer
            PropertyChanges {
                target: orderStatusCB
                enabled: false
            }
        }
    ]

    CustomerInfoBox {
        Layout.fillWidth: true
        customer: root.model.customer
        visible: UserSettings.userTypeId  !== UserEntity.Customer
    }

    Label {
        text: `Estado de Orden ID#${orderId}`
        font.pointSize: 12.5
        font.bold: true
    }

    OrderStatusCB {
        id: orderStatusCB
        orderId: root.orderId
        statusId: status.statusId
        acceptBtn.visible: root.model.taken && !internal.userIsCustomer
        Layout.fillWidth: true
        onStatusUpdated: (newStatusId, newStatusString) => {
            root.model.status.statusId = newStatusId;
            root.model.status.statusString = newStatusString;
        }
    }

    PaymentDetails {
        id: paymentDetails
        payment_: root.payment
        stackView: root.stackView
        taken: root.model.taken
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
        property date orderDate: root.model.date
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

            Component.onCompleted: {
                internal.orderTotal += total;
                console.log(imageUrl)
            }

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

    QtObject {
        id: internal
        readonly property bool userIsCustomer: UserSettings.userTypeId  === UserEntity.Customer
        property int orderTotal
    }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import Cara.Billing
import Cara.User
import Cara.Customer
import Cara.Product
import Cara.Controls

ColumnLayout {
    id: root

    //efault property alias content: root.children
    property T.StackView stackView: StackView.view
    required property var model
    property var payment: root.model.payment
    property var status
    property date date
    property int orderId
    property var details
    property int headerHeight: 20
    readonly property alias paymentDetails: paymentDetails
    readonly property alias commentsDrawer: commentsDrawer

    spacing: 20

    states: [
        State {
            name: "customerView"
            when: UserSettings.userTypeId === UserEntity.Customer
            PropertyChanges {
                orderStatusCB.enabled: false
            }
        }
    ]

    Component.onCompleted: {
        commentsDrawer.fetchCommentsByOrderId(root.orderId);
    }

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
            id: productCard
            required property var product
            required property int quantity
            required property int price
            required total

            Component.onCompleted: {
                internal.orderTotal += total;
            }

            Layout.fillHeight: true
            Layout.fillWidth: true
            enabled: false
            productName: productCard.product.name
            productPrice: productCard.price
            spinBox.value: productCard.quantity
            imageUrl: productCard.product.imageUrl
        }
    }

    RedirectLabel {
        title: "Comentarios"
        Layout.fillWidth: true
        onClicked: commentsDrawer.open()
    }

    CommentsDrawer {
        id: commentsDrawer
        width: ApplicationWindow.window.width
        userId: UserSettings.userId
        orderId: root.orderId
        edge: Qt.BottomEdge
    }

    Item { Layout.fillWidth: true }

    QtObject {
        id: internal
        readonly property bool userIsCustomer: UserSettings.userTypeId  === UserEntity.Customer
        property int orderTotal
    }
}

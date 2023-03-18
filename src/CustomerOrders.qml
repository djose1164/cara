import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils" as Utils
import "qrc:/customers" as Client

// TODO: Update the view after effecting a payment.

Page {
    id: root
    required property var order_details
    required property var payment
    required property string date
    required property int id
    readonly property int orderId: id
    readonly property StackView myStack: StackView.view

    signal paymentApplied

    header: Utils.TitleHeader {
        title: "Estado de Orden"
        label.font.pointSize: 12
        stackView: myStack
    }

    OrderState {
        width: root.width
        anchors {
            top: parent.top
            topMargin: 20
            left: parent.left
            leftMargin: 15
        }
        order_details: root.order_details
        payment: root.payment
        orderId: root.orderId
        date: root.date
        Component.onCompleted: paymentApplied.connect(root.paymentApplied)
    }
}

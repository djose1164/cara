import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils" as Utils
import "qrc:/customers" as Client
import "qrc:/entity"

// TODO: Update the view after effecting a payment.

Page {
    id: root

    required property string name
    required property int customer_id
    required property ListModel orders
    readonly property StackView myStack: StackView.view

    signal paymentApplied(customerPayment: var)

    header: Utils.TitleHeader {
        title: "Cliente: "+name+" ID #"+customer_id;
        label.font.pointSize: 12
        stackView: myStack
    }

    ListView {
        id: listView
        anchors.fill: parent
        anchors.leftMargin: 15
        spacing: 20
        model: orders
        delegate: orderState
    }

    Component {
        id: orderState
        Item{
            id: item
            property alias orderEntity: orderEntity
            required property var payment
            required property string date
            required property int id
            readonly property int orderId: id
            required property var order_details
            required property var order_status

            width: listView.width
            height: listView.height*.6

            OrderEntity {
                id: orderEntity
                payment: item.payment
                date: item.date
                orderId: item.orderId
                orderDetails: item.order_details
                orderStatus: item.order_status
            }

            OrderState {
                anchors.fill: parent
                orderEntity: item.orderEntity
                onPaymentApplied: payment => {
                    root.paymentApplied({payment, customerId: customer_id});
                    const pred = order => order.payment.id === payment.id;
                    for (let i = 0; i < orders.count; ++i)
                        if (pred(orders.get(i)))
                            orders.get(i).payment = payment;
                }
            }
        }
    }
}

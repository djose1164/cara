import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils" as Utils
import "qrc:/customers" as Client

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
        delegate: OrderState {
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

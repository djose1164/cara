import QtQuick
import "qrc:/orders"

ListView {
    id: listView

    spacing: 20

    delegate: OrderState {
        required details
        required orderId
        required date

        width: root.width
        height: root.height*.75

        onPaymentApplied: payment => {
            root.paymentApplied({payment, customerId: customer_id});
            const pred = order => order.payment.id === payment.id;
            for (let i = 0; i < orders.count; ++i)
                if (pred(orders.get(i)))
                    orders.get(i).payment = payment;
        }
    }
}

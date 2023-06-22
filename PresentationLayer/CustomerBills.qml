import QtQuick
import "qrc:/orders"

ListView {
    id: root

    spacing: 20

    delegate: OrderState {
        required details
        required orderId
        required date
        required payment
        required status

        width: root.width
    }
}

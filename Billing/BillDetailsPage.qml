import QtQuick
import QtQuick.Controls
import "qrc:/utils"
import "qrc:/orders"

BackPage {
    id: root

    required property var model
    property int orderId
    property var date
    property var details
    property var payment
    property var status
    property alias customer: orderState.buyer

    title: "Detalles de orden"

    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth

        OrderState {
            id: orderState
            width: {
                if (root.width <= 400)
                    return root.width * .9;
                else if (root.width <= 500)
                    return root.width * .85
                else if (root.width <= 600)
                    return root.width * .8
                else
                    return root.width *.75
            }
            anchors.horizontalCenter: parent.horizontalCenter
            //payment: root.payment
            orderId: root.orderId
            details: root.details
            date: root.date
            status: root.status
            model: root.model
        }
    }
}

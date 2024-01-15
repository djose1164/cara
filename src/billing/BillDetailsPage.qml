import QtQuick
import QtQuick.Controls
import "qrc:/utils"
import "qrc:/orders"

BackPage {
    id: root

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
            anchors {
                fill: parent
                leftMargin: 10
                rightMargin: 10
            }
            payment: root.payment
            orderId: root.orderId
            details: root.details
            date: root.date
            status: root.status
        }
    }
}

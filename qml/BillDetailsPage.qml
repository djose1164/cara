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

    title: "Detalles de orden"

    ScrollView {
        contentWidth: availableWidth
        anchors {
            fill: parent
            leftMargin: 10
            rightMargin: 10
        }

        OrderState {
            anchors.fill: parent
            payment: root.payment
            orderId: root.orderId
            details: root.details
            date: root.date
            status: root.status
        }
    }
}

/* IMPORTANT: This delegate receives orders a model. */
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/users"
import "qrc:/entity"

ItemDelegate {
    id: root

    property int orientation : ListView.Vertical
    readonly property StackView stackView: ListView.view.stackView
    required property var payment
    required property string date
    required property int id
    readonly property int orderId: id
    required property var order_details

    signal updateCustomer(customerId: int)

    width: ListView.view.width
    height: orientation === ListView.Horizontal ? ListView.view.height
                                                : ListView.view.height * .15

    onClicked: stackView.push("qrc:/orders/CustomerOrders.qml",
                              {"orderEntity": orderEntity})

    OrderEntity {
        id: orderEntity
        payment: root.payment
        date: root.date
        orderId: root.orderId
        orderDetails: root.order_details
    }

    ColumnLayout {
        anchors.fill: parent
        Label {
            Layout.leftMargin: 5
            text: `Orden #${root.orderId}`
            font.pointSize: 16
        }
        Label {
            Layout.leftMargin: 15
            text: `**Fecha:** ${root.date}`
            textFormat: Label.MarkdownText
        }
    }
}

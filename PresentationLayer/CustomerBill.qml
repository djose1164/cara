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
    required property var order_status
    readonly property int widthLV: ListView.view.width
    readonly property int heightLV: ListView.view.height
    readonly property bool isHorizontal: orientation === ListView.Horizontal

    signal updateCustomer(customerId: int)

    width: isHorizontal ? widthLV / 2 : widthLV
    height: isHorizontal ? heightLV : heightLV * .15

    onClicked: stackView.push("qrc:/orders/CustomerOrders.qml",
                              {"orderEntity": orderEntity})

    OrderEntity {
        id: orderEntity
        payment: root.payment
        date: root.date
        orderId: root.orderId
        orderDetails: root.order_details
        orderStatus: root.order_status
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

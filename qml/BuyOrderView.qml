pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import "qrc:/orders"
import Cara.Billing

ListView {
    id: root

    required property T.StackView stackView

    delegate: ItemDelegate {
        required property int id
        required property string description
        required property var payment
        required property var order_details
        required property var provider
        required property string date

        onClicked: {
            root.stackView.push("qrc:/orders/BuyOrderDetails.qml", {
                               orderId: id,
                               description,
                               payment,
                               orderDetails: order_details,
                               provider,
                               date,
                           });
        }

        width: root.width
        text: `Order de Compra No.: ${id}\n${description}`
    }

    Label {
        width: root.width * .8
        anchors.centerIn: parent
        horizontalAlignment: Label.AlignHCenter
        visible: root.count === 0
        text: "No hay ordenes de compra para mostrar."
        font.pointSize: 14
        wrapMode: Label.Wrap
    }
}

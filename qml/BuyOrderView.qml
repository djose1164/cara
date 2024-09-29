import QtQuick
import QtQuick.Controls
import "qrc:/orders"
import Cara.Billing

ListView {
    id: listView

    required property StackView stackView

    delegate: ItemDelegate {
        required property int id
        required property string description
        required property var payment
        required property var order_details
        required property var provider
        required property string date

        onClicked: {
            stackView.push("qrc:/orders/BuyOrderDetails.qml", {
                               orderId: id,
                               description,
                               payment,
                               orderDetails: order_details,
                               provider,
                               date
                           });
        }

        width: listView.width
        text: `Order de Compra No.: ${id}\n${description}`
    }

    Label {
        width: listView.width * .8
        anchors.centerIn: parent
        horizontalAlignment: Label.AlignHCenter
        visible: listModel.count === 0
        text: "No hay ordenes de compra para mostrar."
        font.pointSize: 14
        wrapMode: Label.Wrap
    }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils" as Utils
import "qrc:/customers" as Client
import "qrc:/entity"

// TODO: Update the view after effecting a payment.

Page {
    id: root
    required property OrderEntity orderEntity
    readonly property StackView myStack: StackView.view

    signal paymentApplied

    header: Utils.TitleHeader {
        title: "Estado de Orden"
        label.font.pointSize: 12
        stackView: myStack
    }

    OrderState {
        width: root.width
        height: root.height/2
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 15
        orderEntity: root.orderEntity
        Component.onCompleted: paymentApplied.connect(root.paymentApplied)
    }
}

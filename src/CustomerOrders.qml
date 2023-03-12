import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils" as Utils
import "qrc:/customers" as Client

// TODO: Update the view after effecting a payment.

Page {
    id: root
    required property var order_details
    required property var payment
    required property string date
    required property string id
    readonly property StackView myStack: StackView.view

    signal paymentApplied

    header: Utils.TitleHeader {
        title: "Estado de Orden"
        label.font.pointSize: 12
        stackView: myStack
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.leftMargin: 20
        spacing: 20
        Label {
            id: orderLbl
            text: "Orden #" + id
            font.pointSize: 14
            font.bold: true
        }

        Label {
            id: dateBanner
            text: "Fecha"
            font.pointSize: 12
            font.bold: true
        }

        Label { text: date }

        OrderState {
            Layout.fillWidth: true
            Layout.preferredHeight: root.height * .3
            order_details: root.order_details
            payment: root.payment
            Component.onCompleted: paymentApplied.connect(root.paymentApplied)
        }
    }
}

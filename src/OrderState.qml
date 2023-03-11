import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/payment"
import "qrc:/users"

Item {
    id: root
    required property var payment
    required property var order_details
    readonly property var product: order_details.product

    signal paymentApplied

    Component.onCompleted: updatePayment.paymentPatched.connect(paymentApplied);

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        Label {
            text: "Estado de de la orden: "
            font.pointSize: 12
            font.bold: true
        }

        RowLayout {
            Label { text: statusToString(payment.status) }

            Button {
                text: "Actualizar"
                visible: TheCurrentUser.isAdmin
                onClicked: updatePayment.open()
            }
        }

        Label {
            text: "Detalles"
            font.pointSize: 12
            font.bold: true
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 50

            Label { text: "Producto" }

            Label { text: "Cantidad" }
        }

        GridLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 20
            rowSpacing: 1
            columnSpacing: 10
            columns: 2

            Repeater {
                model: order_details
                delegate: Label { text: product.name }
            }
            Repeater {
                model: order_details
                delegate: Label { text: quantity }
            }
        }
    }

    UpdatePayment {
        id: updatePayment
        debt: parseInt(payment.amount_to_pay) - parseInt(payment.paid_amount)
        paid: payment.paid_amount
        paymentID: payment.id
    }

    function statusToString(status) {
        switch(status) {
        case 0:
            return "Pago";
        case 1:
            return "Credito";
        case 2:
            return "Abonado";
        }

        return "";
    }
}

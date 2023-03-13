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
    width: ListView.view ? ListView.view.width : parent.width
    height: ListView.view ? ListView.view.height * .55 : parent.height * .55

    ColumnLayout {
        anchors.fill: parent

        Label {
            text: "Estado de la orden: "
            font.pointSize: 12
            font.bold: true
        }

        RowLayout {
            Layout.topMargin: 0
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

        ColumnLayout {
            id: grid
            RowLayout {
                Layout.fillWidth: true
                spacing: 20
                Label {
                    Layout.alignment: Qt.AlignLeading
                    text: "Producto"
                    font.pointSize: 11
                    font.bold: true
                }
                Label {
                    Layout.alignment: Qt.AlignTrailing
                    text: "Cantidad"
                    font.pointSize: 11
                    font.bold: true
                }
            }
            Repeater {
            model: order_details
            delegate: RowLayout {
                Layout.fillWidth: true
                spacing: 20
                Label {
                    Layout.alignment: Qt.AlignLeading
                    text: product.name
                }
                Label {
                    Layout.alignment: Qt.AlignTrailing
                    text: quantity
                }
            }
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

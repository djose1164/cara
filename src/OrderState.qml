import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/payment"
import "qrc:/users"

Item {
    id: root
    required property int id
    required property var payment
    required property var order_details
    required property string date
    readonly property var product: order_details.product

    signal paymentApplied(newPayment: var)

    onPaymentApplied: p => payment = p

    width: ListView.view ? ListView.view.width : parent.width
    height: ListView.view ? ListView.view.height * .55 : parent.height * .55

    ColumnLayout {
        anchors.fill: parent

        Label {
            text: `Estado de la orden #${id}: `
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
            text: "Fecha"
            font.pointSize: 12
            font.bold: true
        }

        Label {
            text: date
            font.pointSize: 12
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
        payment: root.payment
        Component.onCompleted: paymentPatched.connect(paymentApplied)
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

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/payment"
import "qrc:/users"
import "qrc:/entity"

Item {
    id: root
    required property OrderEntity orderEntity
    readonly property var payment: orderEntity.payment
    readonly property var product: orderEntity.orderDetails.product

    signal paymentApplied(newPayment: var)

    onPaymentApplied: p => payment = p

    ColumnLayout {
        anchors.fill: parent

        Label {
            text: `Estado de Orden ID#${orderEntity.orderId}`
            font.pointSize: 12.5
            font.bold: true
        }

        Label {
            text: orderEntity.orderStatus.status
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
            text: orderEntity.date
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
                    font.pointSize: 12
                    font.bold: true
                }
                Label {
                    Layout.alignment: Qt.AlignTrailing
                    text: "Cantidad"
                    font.pointSize: 12
                    font.bold: true
                }
            }
            Repeater {
            model: orderEntity.orderDetails
            delegate: RowLayout {
                Layout.fillWidth: true
                spacing: 20
                Label {
                    Layout.alignment: Qt.AlignLeading
                    text: product.name
                    font.pointSize: 12
                }
                Label {
                    Layout.alignment: Qt.AlignTrailing
                    text: quantity
                    font.pointSize: 12
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

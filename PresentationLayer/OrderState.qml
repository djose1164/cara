import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/payment"
import "qrc:/singleton"
import com.cara 1.0
import "qrc:/product"

Item {
    id: root

    property PaymentEntity payment
    property date date
    property int orderId
    property OrderModel details

    signal paymentApplied(newPayment: var)

    onPaymentApplied: p => payment = p

    ColumnLayout {
        anchors.fill: parent

        Label {
            text: `Estado de Orden ID#${orderId}`
            font.pointSize: 12.5
            font.bold: true
        }

        Label {
            text: "status"
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

            Repeater {
                id: repeater
                model: details
                delegate: ProductCard {
                    required property ProductEntity product
                    required property int quantity
                    required total

                    enabled: false
                    productName: product.name
                    productPrice: product.price
                    productQuantity: quantity
                    imageUrl: product.imageUrl
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

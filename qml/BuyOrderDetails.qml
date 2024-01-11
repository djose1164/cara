import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"
import "qrc:/product"
import "qrc:/payment"

Page {
    id: root

    readonly property StackView stackView: StackView.view
    property int orderId
    property var payment
    property var orderDetails
    property var provider
    property var contact: provider.contact
    property string description
    property string date

    onPaymentChanged: {
        if (payment.amount_to_pay === payment.paid_amount)
            paymentStatus.getButton("Pago").checked = true;
        else
            paymentStatus.getButton("Credito").checked = true;
    }

    title: "Detalles de Orden"
    header: TitleHeader {
        title: root.title
        onBackClicked: stackView.pop()
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 5

        BuyOrderHeader {
            id: orderHeader
            Layout.fillWidth: true
            provider: root.provider
            description: root.description
            date: root.date
        }

        PaymentStatus {
            id: paymentStatus
            Layout.fillWidth: true
            partialPaid: false
        }

        GroupBox {
            Layout.fillWidth: true
            Layout.fillHeight: true
            title: "Detalles"

            ListView {
                id: listView
                anchors.fill: parent
                spacing: 20
                clip: true
                model: orderDetails
                delegate: ProductCard {
                    required property var product
                    required property int quantity

                    width: listView.width
                    height: 100
                    imageUrl: product.image_url
                    productName: product.name
                    productPrice: product.buy_price
                    spinBox.value: quantity
                    total: product.buy_price * quantity
                }
            }
        }
    }
}

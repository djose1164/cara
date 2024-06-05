import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import Cara.Utils
import Cara.Product
import Cara.Billing
import Cara.Data

Page {
    id: root

    readonly property T.StackView stackView: StackView.view
    property int orderId
    property var payment
    property var orderDetails
    property var provider
    property var contact: provider.contact
    property string description
    property string date

    onPaymentChanged: {
        let button;
        if (payment.amount_to_pay === payment.paid_amount)
            button = paymentStatus.getButtonByText("Pago");
        else
            button = paymentStatus.getButtonByText("Credito");
        button.checked = true;
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
                    imageUrl: product.images && product.images.length ? Constants.uploadsPath + product.images[0].image_url : "qrc:/cara-logo-v1.png"
                    productName: product.name
                    productPrice: product.buy_price
                    spinBox.value: quantity
                    total: product.buy_price * quantity
                }
            }
        }
    }
}

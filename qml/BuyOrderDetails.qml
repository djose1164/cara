pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import Cara.Controls
import Cara.Product
import Cara.Billing
import Cara.Network

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
                    id: productCard
                    required property var product
                    required property int quantity
                    required property double price

                    width: listView.width
                    height: 100
                    enabled: false
                    imageUrl: productCard.product.images
                              && productCard.product.images.length ? Api.uploadsPath + product.images[0].image_url
                                                                   : "qrc:/cara-logo-v1.png"
                    productName: productCard.product.name
                    productPrice: productCard.price
                    spinBox.value: productCard.quantity
                    total: productCard.price * productCard.quantity
                }
            }
        }
    }
}

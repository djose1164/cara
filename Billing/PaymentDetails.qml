import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import Billing
import User
import Utils

Item {
    id: root

    required property payment payment_
    required property T.StackView stackView

    signal refresh(newPayment: payment)

    implicitWidth: gridLayout.implicitWidth
    implicitHeight: groupBox.implicitHeight

    states: [
        State {
            name: "paid"
            when: payment_.status === Payment.Paid
            PropertyChanges {
                target: label
                title: "Ultimo pago"
                content: payment_.updatedAt.toLocaleDateString()
            }
            PropertyChanges {
                target: loader
                sourceComponent: paidDelegate
            }
        },
        State {
            name: "unpaid"
            when: payment_.status === Payment.Unpaid
            PropertyChanges {
                target: label
                title: "Deduda"
                content: `RD$${internal.debt}`
            }
            PropertyChanges {
                target: loader
                sourceComponent: unpaidDelegate
            }
        },
        State {
            name: "credited"
            when: payment_.status === Payment.Credited
            PropertyChanges {
                target: loader
                sourceComponent: creditedDelegate
            }
            PropertyChanges {
                target: label
                title: "Deuda"
                content: `<span>RD$</span><b style="font-size: 14pt;">${internal.debt}</b>`
            }
        }
    ]

    Component.onCompleted: print("paidAmount: "+payment_.paidAmount+" amountToPay: "+payment_.amountToPay)
    GroupBox {
        id: groupBox
        title: "Detalles de Pago"
        anchors.fill: parent

        GridLayout {
            id: gridLayout
            anchors.fill: parent
            columns: 2
            columnSpacing: 25

            Label {
                text: `<span>Estado de Pago</span><h2>${payment_.statusString}</h2>`
                textFormat: Label.RichText
            }

            Label {
                id: label
                property string title
                property string content
                text: `<span>${title}</span><h3>${content}</h3>`
                textFormat: Label.RichText
                wrapMode: Label.Wrap
                horizontalAlignment: Label.AlignRight
                leftPadding: 15
                Layout.fillWidth: true
            }

            Loader {
                id: loader
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.columnSpan: 2
                Layout.preferredWidth: gridLayout.width
                Layout.minimumHeight: 50
            }
        }
    }

    Component {
        id: paidDelegate
        Item {
            implicitWidth: groupBox.width

            SeeMore {
                id: seeMoreBtn
                width: parent.width
            }
        }
    }

    component SeeMore : Button {
        text: "Ver mas"
        onClicked: stackView.push("PaymentDetailsPage.qml", {paymentId: payment_.paymentId})
    }

    Component {
        id: unpaidDelegate
        Item {
            implicitWidth: groupBox.width
            RowLayout {
                anchors.fill: parent
                Button {
                    text: "Pagar"
                    Layout.fillWidth: true
                    onClicked: {
                        payService.pay(internal.debt)
                        popup.state = "busy";
                        popup.open();
                    }
                }

                Button {
                    text: "Abonar"
                    Layout.fillWidth: true
                    onClicked: {
                        popup.state = "credited";
                        popup.open();
                    }
                }
            }
        }
    }

    Component {
        id: creditedDelegate
        Item {
            implicitWidth: groupBox.width
            implicitHeight: creditedGrid.implicitHeight
            GridLayout {
                id: creditedGrid
                anchors.fill: parent
                columns: 2
                rows: 2

                SeeMore {
                    id: seeMoreBtn
                    Layout.fillWidth: true
                }

                Button {
                    text: "Pagar"
                    Layout.fillWidth: true
                    onClicked: {
                        payService.pay(internal.debt)
                        popup.state = "busy";
                        popup.open();
                    }
                }

                Button {
                    id: abonarBtn
                    width: parent.width
                    anchors.margins: 10
                    text: "Abonar"
                    Layout.fillWidth: true
                    Layout.columnSpan: 2
                    onClicked: {
                        popup.state = "credited";
                        popup.open();
                    }
                }
            }
        }
    }

    Popup {
        id: popup

        property alias state: payItem.state

        width: root.width
        height: billStack.height/2
        anchors.centerIn: Overlay.overlay
        onClosed: payItem.resetView()

        PayItem {
            id: payItem
            anchors.fill: parent
            payment_: root.payment_
            onRefresh: newPayment => {
                root.refresh(newPayment);
            }
        }
    }

    RestService {
        Pay {
            id: payService
            paymentId: payment_.paymentId
            path: "payments"
            onPaid: resp => {
                const newPayment = paymentController.fromJsonObject(resp.payment);
                root.refresh(newPayment);
                popup.state = "success";
            }
            onErrorOccurred: popup.state = "errorOccurred"
        }
    }

    PaymentController {
        id: paymentController
    }

    QtObject {
        id: internal
        readonly property int debt: payment_.amountToPay - payment_.paidAmount
    }
}

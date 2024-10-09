pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Templates as T
import Cara.Billing
import Cara.User
import Cara
import Cara.Salesperson

Item {
    id: root

    required property payment payment_
    required property T.StackView stackView
    required property bool taken
    required property int mode
    // If different to -1, we'll evulate whether the salesperson meet the criteria
    // to mark the bill as paid.
    property int salespersonId: -1
    property int orderId: -1

    signal refresh(newPayment: payment)

    implicitWidth: gridLayout.implicitWidth
    implicitHeight: groupBox.implicitHeight

    states: [
        State {
            name: "paid"
            when: root.payment_.status === Payment.Paid
            PropertyChanges {
                label.title: "Ultimo pago"
                label.content: payment_.updatedAt.toLocaleDateString()
            }
            PropertyChanges {
                loader.sourceComponent: paidDelegate
            }
        },
        State {
            name: "unpaid"
            when: root.payment_.status === Payment.Unpaid
            PropertyChanges {
                label.title: "Deuda"
                label.content: `RD$${internal.debt}`
                loader.active: UserSettings.userTypeId !== UserEntity.Customer
                               && (taken || mode === BillDetailsPage.QueueOwnerMode)
                loader.visible: loader.active
                loader.sourceComponent: unpaidDelegate
            }
        },
        State {
            name: "credited"
            when: root.payment_.status === Payment.Credited
            PropertyChanges {
                loader.sourceComponent: creditedDelegate
                label.title: "Deuda"
                label.content: `<span>RD$</span><b style="font-size: 14pt;">${internal.debt}</b>`
            }
        }
    ]

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
                text: `<span>Estado de Pago</span><h2>${root.payment_.statusString}</h2>`
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

    SalespersonController {
        id: spController
        salespersonId: root.salespersonId
        onOrderCanBePaid: payService.pay(internal.debt)
        onErrorOccurred: {
            popup.state = "";
            popup.close();

            const err = JSON.parse(data).message;
            errorMessage.text = err;
            errorMessage.open();
        }
    }

    MessageDialog {
        id: errorMessage
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
        text: "Ver más"
        onClicked: root.stackView.push("PaymentDetailsPage.qml", {paymentId: root.payment_.paymentId})
    }

    component PayButton: Button {
        text: "Pagar"
        Layout.fillWidth: true
        onClicked: {
            popup.state = "busy";
            popup.open();

            if (root.salespersonId !== -1) {
                spController.canPayOrder(root.orderId);
                return;
            }

            payService.pay(internal.debt)
        }
    }

    Component {
        id: unpaidDelegate
        Item {
            implicitWidth: groupBox.width
            implicitHeight: 1
            visible: internal.canPay

            RowLayout {
                anchors.fill: parent

                PayButton {}

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

                PayButton {
                    visible: internal.canPay
                }

                Button {
                    id: abonarBtn
                    visible: internal.canPay
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
        height: root.height * 1.3
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
            paymentId: root.payment_.paymentId
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
        readonly property int debt: root.payment_.amountToPay - root.payment_.paidAmount
        readonly property bool canPay: UserSettings.userTypeId === UserEntity.Admin
                                       || UserSettings.userTypeId === UserEntity.Salesperson
    }
}

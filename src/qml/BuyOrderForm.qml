import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import User
import Salesperson
import Utils
import "qrc:/provider"
import Billing
import "qrc:/utils"
import Product

Drawer {
    id: root

    readonly property var user: UserSession.user
    readonly property var salesperson: UserSession.salesperson
    readonly property var buyOrder: {
        "date": datePicker.toShortDate(),
        "description": descriptionArea.text,
        "provider_id": providerComboBox.currentValue,
        "payment": {
            "amount_to_pay": entry.totalToPay,
            "paid_amount": !paymentStatus.isCredit ? entry.totalToPay : 0
        },
        "total_amount": totalAmount
    }
    property int totalAmount

    signal accepted

    interactive: false

    RowLayout {
        id: header
        width: root.width
        height: 40
        anchors.top: root.top

        Label {
            text: "Nueva Orden de Compra"
            font.pointSize: 16
            font.bold: true
            Layout.leftMargin: 20
        }

        CloseButton {
            id: closeBtn
            onClicked: root.close()
            Layout.alignment: Qt.AlignRight
        }
    }

    ScrollView {
        id: scrollView
        width: root.width
        anchors.top: header.bottom
        anchors.topMargin: 20
        anchors.bottom: parent.bottom
        contentWidth: availableWidth

        ColumnLayout {
            width: root.width*.9
            height: scrollView.height
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 20

            ProviderComboBox {
                id: providerComboBox
                visible: user.userType === UserEntity.Salesperson
                Layout.fillWidth: true
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.preferredHeight: 75

                TextArea {
                    id: descriptionArea
                    placeholderText: "Breve descripcion..."
                    wrapMode: TextArea.Wrap
                }
            }

            RedirectLabel {
                title: datePicker.selectedDate.toLocaleDateString()
                onClicked: datePicker.open()
            }

            DatePicker {
                id: datePicker
                width: root.width * .9
                height: root.height * .75
            }

            PaymentStatus {
                id: paymentStatus
                readonly property bool isCredit: paymentStatus.buttonText === "Credito"

                Layout.fillWidth: true
                partialPaid: false
            }

            Label {
                text: "Poductos"
                font.pointSize: 14
                font.bold: true
                background: Rectangle { color: "white" }
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            AddProductView {
                id: entry
                valueRoleType: ProductComboBox.BuyPrice
                Layout.fillWidth: true
                Layout.preferredHeight: 200
                onEntryUpdated: {
                    totalAmount = entry.calculateTotal();
                    buyOrder.order_details = productEntries.entryValues();
                }
            }

            Label {
                property int total
                text: "Total: RD$" + totalAmount
                font.pointSize: 14
            }

            RowLayout {
                Layout.alignment: Qt.AlignCenter
                spacing: 20

                Button {
                    Layout.columnSpan: 2
                    Layout.alignment: Qt.AlignCenter
                    text: "Agregar Proveedor"
                }

                Button {
                    onClicked: accepted()
                    Layout.fillWidth: true
                    text: "Aceptar"
                    Material.background: Material.Green
                    Material.foreground: "white"
                }
            }
        }
    }
}

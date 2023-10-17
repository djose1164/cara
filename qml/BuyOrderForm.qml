import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import core
import "qrc:/provider"
import "qrc:/payment"
import "qrc:/utils"
import "qrc:/product"

Drawer {
    id: root

    readonly property var buyOrder: {
        "date": dateField.text,
        "admin_id": CurrentUser.user.userId,
        "description": descriptionArea.text,
        "provider_id": providerComboBox.currentValue,
        "payment": {
            "amount_to_pay": entry.totalToPay,
            "paid_amount": !paymentStatus.isCredit ? entry.totalToPay : 0
        },
    }

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

            TextField {
                id: dateField
                Layout.fillWidth: true
                placeholderText: "dia/mes/año"
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
                Layout.fillWidth: true
                Layout.preferredHeight: 200
                onEntryUpdated: { // TODO: Check why property binding doesn't work
                    totalLabel.total = entry.calculateTotal();
                    buyOrder.order_details = productEntries.entryValues();
                }
            }

            Label {
                id: totalLabel
                property int total
                text: "Total: RD$" + total
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

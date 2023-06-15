import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/provider"
import "qrc:/payment"
import "qrc:/utils"

Drawer {
    id: root

    readonly property var buyOrder: {
        "date": dateField.text,
        "description": descriptionArea.text,
        "provider_id": providerComboBox.currentValue,
        "payment": {
            "amount_to_pay": entry.total,
            "paid_amount": !paymentStatus.isCredit ? entry.total : 0
        },
        "order_details": entry.productEntries.entryValues()
    }

    signal accepted

    interactive: false

    CloseButton {
        id: closeBtn
        onClicked: root.close()

        anchors {
            top: parent.top
            right: parent.right
        }
    }

    ColumnLayout {
        height: parent.height
        anchors {
            left: parent.left
            leftMargin: 20
            right: parent.right
            rightMargin: 20
        }
        spacing: 20

        Label {
            text: "Nueva Orden de Compra"
            Layout.topMargin: 5
            font.pointSize: 16
            font.bold: true
        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: childrenRect.height

            Label {
                id: providersLbl
                anchors.verticalCenter: providerComboBox.verticalCenter
                text: "Proveedores"
                font.pointSize: 12
            }

            ProviderComboBox {
                id: providerComboBox
                width: root.width/2
                anchors {
                    left: providersLbl.right
                    leftMargin: 20
                }
            }
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.preferredHeight: 60

            TextArea {
                id: descriptionArea
                placeholderText: "Breve descripcion..."
            }
        }

        TextField {
            id: dateField
            Layout.fillWidth: true
            placeholderText: "dia/mes/año"
        }

        PaymentStatus {
            id: paymentStatus
            readonly property bool isCredit: checkedButton.text === "Credito"

            Layout.fillWidth: true
            partialPaid: false
        }

        Label {
            text: "Poductos"
            font.pointSize: 14
            font.bold: true
        }

        Description {
            id: entry
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Label {
            id: totalLabel
            text: "Total: RD$" + entry.total
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

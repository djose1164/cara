import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import core
import User
import Salesperson
import "qrc:/payment" as Payments
import "qrc:/customers"
import "qrc:/singleton"
import "qrc:/product"

Page {
    id: root

    required property var salesperson

    readonly property bool isACreditOrder: paymentStatus.buttonText === "Credito"
    readonly property bool isPartiallyPaid: paymentStatus.buttonText === "Abono"
    readonly property bool isPaid: paymentStatus.buttonText === "Pago"
    readonly property int amount: {
        if (isPartiallyPaid && amountField.text)
            return parseInt(amountField.text);
        else if (isPaid)
            return addProductView.totalToPay;
        else
            return 0;
    }
    readonly property int total: amountField.text ? addProductView.totalToPay - amount : addProductView.totalToPay
    readonly property var user: UserSession.user

    header: AddOrderHeader {}
    states: [
        State {
            name: "empty"
            PropertyChanges {
                target: clientField
                currentIndex: -1
            }
            PropertyChanges {
                target: paymentStatus
                checkState: Qt.Unchecked
            }
            PropertyChanges {
                target: amountField
                text: ""
            }
            PropertyChanges {
                target: dateField
                text: ""
            }
            StateChangeScript {
                name: "clearAddProductView"
                script: addProductView.clear()
            }
        }
    ]

    ScrollView {
        anchors.fill: parent
        contentWidth: root.width

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 10

            ClientComboBox {
                id: clientField
                Layout.fillWidth: true
                Layout.preferredHeight: clientField.implicitHeight *1.2
            }

            AddProductView {
                id: addProductView
                valueRoleType: ProductComboBox.SellPrice
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumHeight: 200
            }

            Payments.PaymentStatus {
                id: paymentStatus
                Layout.alignment: Qt.AlignCenter
                Layout.fillWidth: true
                Layout.topMargin: 40
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                TextField {
                    id: amountField
                    placeholderText: getPlaceholderText()
                    enabled: isPartiallyPaid
                    inputMethodHints: Qt.ImhDigitsOnly
                    Layout.alignment: Qt.AlignLeading
                    Layout.fillWidth: true
                }

                Label {
                    id: amountLabel
                    visible: isPartiallyPaid
                    text: `Debera: RD$${total}`
                    font.pointSize: 12
                }
            }

            TextField {
                id: dateField
                Layout.columnSpan: 2
                Layout.fillWidth: true
                placeholderText: "dia/mes/año"
                validator: RegularExpressionValidator {
                    regularExpression: /^([0-2][0-9]|(3)[0-1])(\/)(((0)[0-9])|((1)[0-2]))(\/)\d{4}$/
                }
                inputMethodHints: Qt.ImhNoPredictiveText
            }

            Button {
                id: okBtn
                text: "Aceptar"
                onClicked: if (validateForm()) okHandler()
                Layout.fillWidth: true
                Layout.bottomMargin: 30
            }
        }
    }

    OrderAPI {
        id: request

        onOrderCreated: function(r) {
            message.text = "Orden realizada con exito";
            message.informativeText = "¡Gracias por su compra!";
            message.detailedText = "Puedes observar el estado de la orden en Ordenes Recientes.";
            message.open();

            root.state = "empty";
        }

        onFailed: function(r) {
            message.text = "No se pudo realizar la accion";
            message.informativeText = "Error: " + r.message;
            message.open();
        }
    }

    MessageDialog {
        id: message

        onButtonClicked: function(btn, role) {
            switch (btn) {
            case MessageDialog.Ok:
                close();
                break;
            }
        }

        text: "Datos Invalidos!"
    }

    function okHandler() {
        root.state = "";
        const fullOrder = {
            customer_id: clientField.currentValue,
            salesperson_id: user.salespersonId,
            details: addProductView.entryFormData(),
            payment: {
                amount_to_pay: addProductView.totalToPay,
                paid_amount: amount
            }
        };
        print(addProductView.entryFormData())
        if (dateField.length)
            fullOrder.date = dateField.text;

        request.post("/", fullOrder);
    }

    function validateForm() {
        if (clientField.currentIndex === -1) {
            message.informativeText = "Debes seleccionar un cliente al cual adjuntarle"
            + " la ordern."
            message.open();
            return false;
        } else if (!addProductView.productEntries.selectedProductsAreValid()) {
            message.informativeText = "Debes seleccionar un producto valido.";
            message.open();
            return false;
        } else if (paymentStatus.checkState === Qt.Unchecked) {
            message.informativeText = "Debes seleccionar el estado de la orden.";
            message.open();
            return false;
        }

        return true;
    }

    function getPlaceholderText() : string {
        if (isPaid || isACreditOrder)
            return `Monto a pagar: ${total}`;
        else if (!paymentStatus.checkedButton)
            return "Seleccione un estado";
        else
            return "Ingrese monto...";
    }
}

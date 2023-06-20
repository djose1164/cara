import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import com.cara
import "qrc:/payment" as Payments
import "qrc:/customers"
import "qrc:/singleton"
import "qrc:/product"

Page {
    id: root
    readonly property bool isACreditOrder: paymentStatus.buttonText === "Credito"
    readonly property bool isCredited: paymentStatus.buttonText === "Abono"
    readonly property bool isPaid: paymentStatus.buttonText === "Pago"
    readonly property int amount: amountField.text ? amountField.text : 0
    readonly property int total: amountField.text ? addProductView.totalToPay - amount : addProductView.totalToPay

    header: AddOrderHeader {}

    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 10

            ClientComboBox {
                id: clientField
                Layout.fillWidth: true
                z: 100
            }

            AddProductView {
                id: addProductView
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.topMargin: 20
                Layout.minimumHeight: 250
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
                    placeholderText: isPaid || isACreditOrder ? `Monto a pagar: ${total}`
                                                              : !paymentStatus.checkedButton ? "Seleccione un estado"
                                                                                             : "Ingrese monto..."
                    enabled: isCredited
                    inputMethodHints: Qt.ImhDigitsOnly
                    Layout.alignment: Qt.AlignLeading
                    Layout.fillWidth: true
                }

                Label {
                    id: amountLabel
                    visible: isCredited
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
            addProductView.clearOrderForm();

            if (TheCurrentUser.isCostumer && TheCurrentUser.customerId === 0) {
                TheCurrentUser.customerId = r.customer_id;
                CurrentUser.customerId = r.customer_id;
            }
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
        const fullOrder = {
            customer_id: clientField.currentValue,
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
}

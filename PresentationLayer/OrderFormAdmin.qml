import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
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
    readonly property int total: amountField.text ? addOrder.totalToPay - amount : addOrder.totalToPay

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
                id: addOrder
                Layout.fillWidth: true
                Layout.topMargin: 20
                Layout.minimumHeight: 250
            }

            Payments.PaymentStatus {
                id: paymentStatus
                Layout.alignment: Qt.AlignCenter
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
                onClicked: validateForm()
                Layout.fillWidth: true
                Layout.bottomMargin: 30
            }
        }
    }

    MessageDialog {
        id: message
        text: "Datos Invalidos!"
    }

    function okHandler() {
        const details = addOrder.entryFormData();

        const fullOrder = {
            customer_id: clientField.currentValue,
            details,
            payment: {
                amount_to_pay: addOrder.totalToPay,
                paid_amount: amount
            }
        };

        if (dateField.length)
            fullOrder.date = dateField.text;
        print('fullOrder ', JSON.stringify(fullOrder, null, 2));
        addOrder.request.post("/", fullOrder);
    }

    function validateForm() {
        if (clientField.currentIndex === -1) {
            message.informativeText = "Debes seleccionar un cliente al cual adjuntarle"
            + " la ordern."
            message.open();
            return false;
        } else
        if (paymentStatus.checkState === Qt.Unchecked) {
            message.informativeText = "Debes seleccionar el estado de la orden.";
            message.open();
            return false;
        } else
        if (!addOrder.productEntries.selectedProductsAreValid()) {
            message.informativeText = "Debes seleccionar un producto valido.";
            message.open();
            return false;
        }

        return true;
    }
}

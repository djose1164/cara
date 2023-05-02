import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

// TODO: Make two views: one for just seeing & the another for editing.
Popup {
    id: root

    // Any previous payment from the client
     property var payment
    readonly property int debt: amountToPay - paidAmount
    readonly property int amountToPay: payment.amount_to_pay
    readonly property int paidAmount: payment.paid_amount
    readonly property int paid: payment.paid_amount
    property alias newPayment: newPayment.text
    // NOTE: A special case may happen here, when the client pays more than the debt;
    // if this happens, the negative amount should be saver for future buys from this
    // client.
    // property int newState: debt - parseInt(newPayment.text) == 0 ? 0 : 2
    property int newState: newPayment.text.trim() === "" ? 2 : (debt - parseInt(newPayment.text) === 0 ? 0 : 2)

    signal paymentPatched(newPayment: var)

    modal: true
    anchors.centerIn: Overlay.overlay

    onClosed: payment.status ? stackLayout.currentIndex = 1 : stackLayout.currentIndex = 3

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        currentIndex: payment.status ? 1 : 3

        BusyIndicator {}

        ColumnLayout {
            spacing: 10
            Layout.alignment: Qt.AlignCenter

            Label { text: "La deuda asciende a RD$" + debt }

            Label { text: "El cliente ha abonado RD$" + paid }

            TextField {
                id: newPayment
                placeholderText: "Ingrese monto a abonar"
                Layout.fillWidth: true
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 50

                Button {
                    text: "Pagado"
                    onClicked: paymentRequest.setAsPaid()
                    Layout.alignment: Qt.AlignLeading
                }

                Button {
                    text: "Ok"
                    onClicked: parseInt(newPayment.text)
                               > debt ? (stackLayout.currentIndex
                                         = 1) : paymentRequest.patchPayment(
                                            parseInt(newPayment.text) + paid)
                    Layout.alignment: Qt.AlignRight
                }
            }
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignCenter
            Label {
                text: "¡Cambios realizados sastifactoriamente!"
                Layout.alignment: Qt.AlignCenter
            }

            IconImage {
                source: "qrc:/icons/done.svg"
                sourceSize: "48x48"
                Layout.alignment: Qt.AlignCenter
            }
        }

        GridLayout {
            columns: 2
            rowSpacing: 25

            Label {
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignTop
                text: "¿Deseas cambiar el estado de esta orden?"
            }

            Label {
                Layout.columnSpan: 2
                text: "El nuevo estado pasara a hacer: credito."
            }

            Button {
                Layout.alignment: Qt.AlignHCenter
                text: "Cancelar"
                onClicked: close()
            }

            Button {
                Layout.alignment: Qt.AlignHCenter
                text: "Ok"
                onClicked: {
                    newPayment.text = "0";
                    paymentRequest.patchPayment(0);
                }
            }
        }
    }

    Payment {
        id: paymentRequest
        payment: root.payment
        onRequested: stackLayout.currentIndex = 0
        onPaymentPatched: p => {
            root.payment = p;
            root.paymentPatched(p);
            clean();
            stackLayout.currentIndex = 2;
            print(`root.payment = p: ${JSON.stringify(root.payment, null, 2)}`);
        }
    }

    function clean() {
        newPayment.text = "";
    }
}

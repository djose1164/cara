import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import com.cara 1.0

// TODO: Make two views: one for just seeing & the another for editing.
Popup {
    id: root

    // Any previous payment from the client
    property int amountToPay
    property int paidAmount
    property int paymentStatus
    property int paymentId

    readonly property int amountInput: parseInt(newPayment.text)
    readonly property int debt: amountToPay - paidAmount
    property alias newPayment: newPayment.text
    readonly property int wantsToPay: amountInput + root.paidAmount
    // NOTE: A special case may happen here, when the client pays more than the debt;
    // if this happens, the negative amount should be saver for future buys from this
    // client.
    // property int newState: debt - parseInt(newPayment.text) == 0 ? 0 : 2
    property int newState: newPayment.text.trim() === "" ? 2 : (debt - parseInt(newPayment.text) === 0 ? 0 : 2)

    signal paymentPatched(newPaidAmount: int)

    modal: true
    anchors.centerIn: Overlay.overlay

    onClosed: payment.status ? stackLayout.currentIndex = 1 : stackLayout.currentIndex = 3

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        currentIndex: paymentStatus ? 1 : 3

        BusyIndicator {}

        ColumnLayout {
            spacing: 10
            Layout.alignment: Qt.AlignCenter

            Label { text: "La deuda asciende a RD$" + debt }

            Label { text: "El cliente ha abonado RD$" + paidAmount }

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
                                         = 1) : paymentRequest.patch()
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

    PaymentBusiness {
        id: paymentRequest
        payment: paymentEntity
        onRequested: stackLayout.currentIndex = 0
        onPatchSucceeded: {
            paidAmount = wantsToPay;
            clean();
            paymentPatched(wantsToPay);
            stackLayout.currentIndex = 2;

        }
    }

    PaymentEntity {
        id: paymentEntity
        paymentId: root.paymentId
        paidAmount: amountInput + root.paidAmount
        amountToPay: root.amountToPay
    }

    function clean() {
        newPayment.text = "";
    }
}

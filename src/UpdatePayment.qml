import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

// TODO: Make two views: one for just seeing & the another for editing.
Popup {
    id: root

    required property int debt
    required property int paid
    // Any previous payment from the client
    required property int paymentID
    property alias newPayment: newPayment.text
    // NOTE: A special case may happen here, when the client pays more than the debt;
    // if this happens, the negative amount should be saver for future buys from this
    // client.
    // property int newState: debt - parseInt(newPayment.text) == 0 ? 0 : 2
    property int newState: newPayment.text.trim() === "" ? 2 : (debt - parseInt(newPayment.text) === 0 ? 0 : 2)

    signal paymentPatched()

    modal: true
    width: parent.width * .9
    height: parent.height / 2
    anchors.centerIn: Overlay.overlay

    onClosed: if (stackLayout.currentIndex === 2) stackLayout.currentIndex = 1

    Component.onCompleted: print("UpdatePayment: debt: " + debt)

    StackLayout {
        id: stackLayout
        anchors.centerIn: parent
        currentIndex: debt ? 1 : 3

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

        IconImage { source: "qrc:/icons/done.svg" }

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
                    close();
                }
            }
        }
    }

    Payment {
        id: paymentRequest
        paymentId: paymentID
        debt: root.debt
        onRequested: stackLayout.currentIndex = 0
        onPaymentPatched: paymentSucceeded()
    }

    function clean() {
        newPayment.text = "";
    }

    function paymentSucceeded() {
        root.paymentPatched();
        stackLayout.currentIndex = 2;
    }
}

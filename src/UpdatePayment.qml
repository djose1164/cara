import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import "qrc:/payment"

// TODO: Make two views: one for just seeing & the another for editing.
Popup {
    required property int debt
    required property int paid // Any previous payment from the client
    required property int paymentID
    property alias newPayment: newPayment.text
    // NOTE: A special case may happen here, when the client pays more than the debt;
    // if this happens, the negative amount should be saver for future buys from this
    // client.
    property int newState: debt - parseInt(newPayment.text) == 0 ? 0 : 2

    modal: true
    width: parent.width * 2
    height: parent.height * .8
    anchors.centerIn: Overlay.overlay

    StackLayout {
        id: stackLayout
        anchors.centerIn: parent
        currentIndex: debt ? 0 : 2
        
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
                Button {
                    text: "Ok"
                    onClicked: parseInt(newPayment.text) > debt ? (stackLayout.currentIndex = 1) : requestPatch()
                    Layout.alignment: Qt.AlignHCenter
                }

                Button {
                    text: "Pagado"
                    onClicked: {
                        newPayment.text = debt;
                        requestPatch();
                    }
                }
            }
        }

        Label {
            id: stateLbl
            text: "Monto introducido excede la deuda"
            font.bold: true
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
                    requestPatch();
                    close();
                }
            }
        }
    }

    Payment {
        id: paymentRequest
        newPaidAmount: parseInt(newPayment.text)
        paymentId: paymentID
        onMessageChanged: stateLbl.text = message
    }

    function requestPatch() {
        paymentRequest.patchPayment();
        if (!paymentRequest.successful)
            stackLayout.currentIndex = 1;
    }
}

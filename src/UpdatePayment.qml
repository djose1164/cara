import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/payment"

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
    width: parent.width * 1.5
    height: parent.height * .8
    anchors.centerIn: Overlay.overlay

    StackLayout {
        id: stackLayout
        anchors.centerIn: parent
        ColumnLayout {
            spacing: 10
            Layout.alignment: Qt.AlignCenter

            Label { text: "La deuda asciende a RD$" + debt }

            Label { text: "El cliente ha abonado RD$" + paid }

            TextField {
                id: newPayment
                placeholderText: "Ingrese monto"
            }

            Button {
                text: "Ok"
                onClicked: parseInt(newPayment.text) > debt ? stackLayout.currentIndex = 1 : requestPatch()
                Layout.alignment: Qt.AlignHCenter
            }
        }

        Label {
            id: stateLbl
            text: "Monto introducido excede la deuda"
            font.bold: true
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

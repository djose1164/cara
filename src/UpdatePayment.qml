import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    property int debt: 0
    property alias newPayment: newPayment.text
    // NOTE: A special case may happen here, when the client pays more than the debt;
    // if this happens, the negative amount should be saver for future buys from this
    // client.
    property int newState: debt - newPayment == 0 ? 0 : 2

    modal: true
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        
        TextField {
            id: newPayment
            placeholderText: "Nuevo monto"
        }
        
        Label { text: newState == 0 ? "Cliente debera $RD" + (debt - newPayment) : "¡Cuenta saldada!" }
    }
}

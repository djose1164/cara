import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    Layout.columnSpan: 2
    required property int paymentState

    RadioButton {
        checked: true
        text: "Pago"
        onClicked: payment = 0
    }

    RadioButton {
        text: "Credito"
        onClicked: payment = 1
    }

    RadioButton {
        text: "Abono"
        onClicked: payment = 2
    }
}

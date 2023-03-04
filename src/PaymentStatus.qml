import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    property alias group: group

    ButtonGroup {
        id: group
        buttons: children
    }

    RadioButton {
        checked: true
        text: "Pago"
    }

    RadioButton { text: "Credito" }

    RadioButton { text: "Abono" }
}

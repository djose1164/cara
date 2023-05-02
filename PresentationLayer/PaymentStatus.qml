import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    property alias checkedButton: group.checkedButton

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

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    id: root

    property alias checkedButton: group.checkedButton
    property alias partialPaid: partial.enabled
    property alias buttons: group.buttons
    property alias credited: creditBtn.checked
    property alias paid: paidBtn.checked

    height: 40

    ButtonGroup {
        id: group
        buttons: root.children
    }

    RadioButton {
        id: paidBtn
        checked: true
        text: "Pago"
    }

    RadioButton {
        id: creditBtn
        text: "Credito"
    }

    RadioButton {
        id: partial
        text: "Abono"
    }
}


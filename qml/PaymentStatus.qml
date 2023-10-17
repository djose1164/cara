import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

GroupBox {
    id: root

    property alias checkedButton: group.checkedButton
    property alias partialPaid: partial.enabled
    property alias buttons: group.buttons
    property alias credited: creditBtn.checked
    property alias paid: paidBtn.checked
    property alias checkState: group.checkState
    readonly property string buttonText: checkedButton ? checkedButton.text : ""

    width: 80
    height: 20
    title: "Condiicon de Pago"

    RowLayout {
        id: rowLayout
        anchors.centerIn: parent

        ButtonGroup {
            id: group
            buttons: rowLayout.children
        }

        RadioButton {
            id: paidBtn
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
}


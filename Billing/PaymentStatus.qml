import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import entities

Item {
    id: root

    readonly property alias checkedButton: group.checkedButton
    readonly property alias selectedStatus: internal.selectedStatus
    readonly property string buttonText: checkedButton ? checkedButton.text : ""
    property alias checkState: group.checkState
    property alias partialPaid: partial.enabled

    width: 80
    height: rowLayout.childrenRect.height * 1.25

    GroupBox {
        anchors.fill: parent
        title: "Condicion de Pago"

        ButtonGroup {
            id: group
            buttons: rowLayout.children
            onClicked: function(btn) {
                const name = btn.text;
                if (name === "Pago")
                    internal.selectedStatus = PaymentEntity.Paid;
                else if (name === "Credito")
                    internal.selectedStatus = PaymentEntity.Unpaid;
                else if (name === "Abono")
                    internal.selectedStatus = PaymentEntity.Credited;
            }
        }

        RowLayout {
            id: rowLayout
            anchors.centerIn: parent

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

        QtObject {
            id: internal
            property int selectedStatus: -1
        }
    }

    function getButtonByText(text: string): AbstractButton {
        for (let btn of group.buttons)
            if (btn.text === text)
                return btn;
        return null;
    }

    function getButton(text: string) {
        for (let btn of group.buttons)
            if (btn.text === text)
                return btn;

        return null;
    }
}

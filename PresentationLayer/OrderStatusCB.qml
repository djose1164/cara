import QtQuick
import QtQuick.Controls
import Qt.labs.platform
import "qrc:/utils"

Item {
    id: root

    property int orderId
    property int statusId

    signal statusUpdated(newStatusId: int, newStatusString: string)

    width: 100
    height: 40

    ComboBox {
        id: statusComboBox

        width: root.width*.7
        currentIndex: statusId - 1
        textRole: "text"
        valueRole: "value"
        model: [
            {text: "Pendiente", value: 1},
            {text: "Procesando", value: 2},
            {text: "Enviado", value: 3},
            {text: "Entregado", value: 4},
            {text: "Cancelado", value: 5},
            {text: "En Espera", value: 6},
        ]
    }

    Button {
        onClicked: {
            if (statusComboBox.currentValue === statusId)
                return;
            busyPopup.open();
            api.update(statusComboBox.currentValue)
        }

        width: root.width*.25
        anchors {
            right: root.right
            rightMargin: 10
        }
        text: "Aceptar"
        Material.background: Material.Green
        Material.foreground: "white"
    }

    MessageDialog {
        id: msgDialog
        text: "El estatus de la orden ha sido actualizado"
        informativeText: "Si el cliente cuenta con un usuario, el mismo sera notificado."
        onAccepted: close()
    }

    Popup {
        id: busyPopup
        width: 200
        height: 100
        anchors.centerIn: Overlay.overlay
        modal: true

        BusyIndicator { anchors.centerIn: parent }
    }

    CaraApi {
        id: api

        onSucceeded: {
            busyPopup.close();
            msgDialog.open();

            statusUpdated(statusComboBox.currentValue, statusComboBox.currentText);
        }

        endpoint: "/statuses"

        function update(newOrderStatus: int) {
            patch(`/${orderId}`, {"status_id": newOrderStatus});
        }
    }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

import Utils


// TODO: This component need to be reviewed

Item {
    id: root

    property int orderId
    property int statusId

    signal statusUpdated(newStatusId: int, newStatusString: string)

    width: 100
    height: 40

    ComboBox {
        id: statusComboBox

        width: root.width*.6
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
        anchors {
            left: statusComboBox.right
            leftMargin: 20
            right: root.right
            rightMargin: 10
        }
        visible: root.enabled
        text: "Aceptar"
        onClicked: {
            if (statusComboBox.currentValue === statusId)
                return;
            busyPopup.open();
            api.update(statusComboBox.currentValue)
        }
    }

    MessageDialog {
        id: msgDialog
        text: "El estatus de la orden ha sido actualizado"
        informativeText: "Si el cliente cuenta con un usuario, el mismo sera notificado."
        buttons: MessageDialog.Ok
        onButtonClicked: close()
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

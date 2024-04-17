import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import Utils

// TODO: This component need to be reviewed
Item {
    id: root

    property int orderId
    property int statusId

    readonly property alias acceptBtn: acceptBtn

    signal statusUpdated(newStatusId: int, newStatusString: string)

    width: 100
    height: 40

    states: [
        State {
            name: "failed"
            PropertyChanges {
                target: msgDialog
                title: "Error"
                text: internal.errorText
            }
        }
    ]

    Component.onCompleted: {
        internal.oldStatus = root.statusId;
    }

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
        id: acceptBtn
        anchors {
            left: statusComboBox.right
            leftMargin: 20
            right: root.right
            rightMargin: 10
        }
        enabled: statusComboBox.currentValue !== internal.oldStatus
        text: "Aceptar"
        onClicked: {
            busyPopup.open();
            api.update(statusComboBox.currentValue)
        }
    }

    MessageDialog {
        id: msgDialog
        title: "Estatus actualizado con exito"
        text: "Si el cliente cuenta con un usuario, el mismo sera notificado."
        buttons: MessageDialog.Ok
        onButtonClicked: {
            root.state = "";
            msgDialog.close();
        }
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

            internal.oldStatus = statusComboBox.currentValue;
            console.log(`${internal.oldStatus} = ${statusComboBox.currentValue}`)
            statusUpdated(statusComboBox.currentValue, statusComboBox.currentText);
        }
        onFailed: resp => {
            internal.errorText = resp.message;
            root.state = "failed";
            busyPopup.close();
            msgDialog.open();
        }

        endpoint: "/statuses"

        function update(newOrderStatus: int) {
            patch(`/${orderId}`, {"status_id": newOrderStatus});
        }
    }

    QtObject {
        id: internal
        property string errorText
        property int oldStatus
    }
}

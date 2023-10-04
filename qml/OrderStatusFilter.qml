import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import core
import entities
import "qrc:/utils"

Popup {
    id: root

    signal applyFilter(statusId: int)

    anchors.centerIn: Overlay.overlay

    GridLayout {
        anchors.fill: parent
        columns: 2

        Label {
            text: "Filtrar cuentas"
            font.pointSize: 14
        }

        CloseButton {
            Layout.alignment: Qt.AlignRight
            onClicked: close()
        }

        Label {
            text: "Seleccionar estatus"
        }

        ComboBox {
            id: comboBox
            textRole: "text"
            valueRole: "value"
            model: [
                {text: "Pendiente", value: OrderStatus.Pending},
                {text: "Procesando", value: OrderStatus.Processing},
                {text: "Enviado", value: OrderStatus.Sent},
                {text: "Entregado", value: OrderStatus.Delivered},
                {text: "Cancelado", value: OrderStatus.Canceled},
                {text: "En Espera", value: OrderStatus.OnHold},
                {text: "Todas", value: OrderStatus.Any},
            ]
            currentIndex: comboBox.count - 1
        }

        Button {
            text: "Aceptar"
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignCenter
            onClicked: {
                applyFilter(comboBox.currentValue);
                close();
            }
        }
    }
}

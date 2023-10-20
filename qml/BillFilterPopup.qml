import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import entities

Dialog {
    id: root

    readonly property alias paymentFilterValue: paymentComboBox.currentValue
    readonly property alias orderFilterValue: orderComboBox.currentValue

    anchors.centerIn: Overlay.overlay
    title:  "Filter ordenes"
    standardButtons: Dialog.Ok | Dialog.Cancel

    GridLayout {
        anchors.fill: parent
        columns: 2
        Label {
            text: "Estado de cuenta"
        }

        ComboBox {
            id: paymentComboBox
            textRole: "text"
            valueRole: "value"
            model: [
                {text: "Cuentas pagadas", value: PaymentEntity.Paid},
                {text: "Cuentas por pagar", value: PaymentEntity.Unpaid},
                {text: "Cuentas abonadas", value: PaymentEntity.Credited},
                {text: "Todas", value: PaymentEntity.Any}
            ]
            Layout.fillWidth: true
            flat: true
            currentIndex: 3
        }

        Label {
            text: "Seleccionar estatus"
        }

        ComboBox {
            id: orderComboBox
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
            currentIndex: count - 1
        }
    }
}

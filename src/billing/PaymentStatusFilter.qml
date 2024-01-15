import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import core
import entities
import Customer

Popup {
    id: root

    signal applyFilter(filter: int)

    GridLayout {
        anchors.fill: parent
        columns: 2
        rowSpacing: 20

        Label {
            text: "Filtrar Ordenes"
            horizontalAlignment: Label.AlignHCenter
            font.pointSize: 16
            Layout.columnSpan: 2
            Layout.fillWidth: true
        }

        Label {
            text: "Estado de cuenta"
        }

        ComboBox {
            id: comboBox
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

        Button {
            text: "Cancelar"
            Layout.leftMargin: 20
            Layout.alignment: Qt.AlignRight
            onClicked: close()
        }

        Button {
            text: "Aceptar"
            Layout.rightMargin: 20
            Layout.alignment: Qt.AlignRight
            onClicked: {
                applyFilter(comboBox.currentValue);
                close()
            }
        }
    }
}

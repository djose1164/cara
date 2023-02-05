import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/client"

Drawer {
    id: root

    required property ClientModel clientModel

    width: parent.width
    height: parent.height
    interactive: false

    GridLayout {
        Layout.fillWidth: true
        columns: 2
        rowSpacing: 20

        Button {
            background: Image {
                source: "qrc:/left_arrow"
                sourceSize: "24x24"
            }
            Layout.columnSpan: 2
        }

        Label {
            text: "Filtro"
            font.pointSize: 20
            font.bold: true
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignHCenter
            Layout.bottomMargin: 20
        }

        Label { text: "Ordenar por" }

        ComboBox {
            id: orderByCB
            textRole: "text"
            valueRole: "value"
            model: [
                {text: "Fecha", value: "date"}
            ]
            flat: true
        }

        Label { text: "Estado" }

        ComboBox {
            id: statusCB
            textRole: "text"
            valueRole: "value"
            model: [
                {text: "Cuentas pagadas", value: 0},
                {text: "Cuentas por pagar", value: 1},
                {text: "Cuentas abonadas", value: 2}
            ]
            Layout.fillWidth: true
            flat: true
        }

        Button {
            text: "Cancelar"
            onClicked: close()
            Layout.alignment: Qt.AlignRight
        }

        Button {
            text: "Aplicar"
            onPressed: makeRequest()
            onClicked: close()
            Layout.alignment: Qt.AlignRight
        }
    }

    function makeRequest() {
        clientModel.filter({
                "type": "filter",
                "value": {
                    "order_by": orderByCB.currentValue,
                    "filter_by": "date",
                    "status": statusCB.currentValue,
                    "model": clientModel.model
                }
            });
    }
}

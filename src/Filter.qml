import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/client" as Client

Drawer {
    property ListModel listModel

    id: root
    width: parent.width
    height: parent.height
    interactive: false

    GridLayout {
        width: parent.width/2
        height: width
        anchors {
            left: parent.left
            leftMargin: width * .30
        }
        columns: 2
        rowSpacing: 25

        Button {
            background: Image {
                source: ":/crossIcon"
            }
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
            Layout.bottomMargin: 20
            flat: true
        }

        Button {
            text: "Cancelar"
            onClicked: close()
            Layout.alignment: Qt.AlignRight
        }

        Button {
            text: "Aplicar"
            onPressed: {
                request.filter({
                        "type": "filter",
                        "value": {
                            "orderBy": orderByCB.currentValue,
                            "status": statusCB.currentValue,
                            "model": listModel
                        }
                    });
            }

            onClicked: close()
            Layout.alignment: Qt.AlignRight
        }
    }

    Client.ClientRequest {
        id: request
        model: root.listModel
    }
}

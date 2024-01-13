import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/customers"

Drawer {
    id: root

    property CustomerModel customerModel

    signal filterChanged

    width: parent.width
    height: parent.height
    interactive: false

    GridLayout {
        width: parent.width*.85
        height: parent.height > parent.width ? parent.height/2 : parent.height
        anchors.centerIn: parent
        columns: 2
        rowSpacing: 20

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
                {text: "ID", value: "id"},
                {text: "Nombre", value: "name"},
                // {text: "Fecha", value: "date"}, For latest purchase
            ]
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
                close();
            }
            Layout.alignment: Qt.AlignRight
        }
    }

    ListModel { id: filteredData }

}

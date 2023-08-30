import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/customers"
import "qrc:/users"

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
                {text: "Fecha", value: "date"}
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

    CustomerAPI {
        id: customerApi

        function filterCustomersByOrderStatus(status: int) {
            rest.get(`/?filter_by=order_status&status=${status}`);
            rest.succeeded.connect(resp => filtered(resp.customers));
        }
    }

    ListModel { id: filteredData }

}

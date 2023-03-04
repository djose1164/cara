import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/customers"

Drawer {
    id: root

    required property CustomerModel customerModel
    enum Status { Paid, Unpaid, Credited, Any }

    signal filtered(model: ListModel)

    width: parent.width
    height: parent.height
    interactive: false

    QtObject {
        id: _
        property int previousFilter: -1
    }

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

        Label { text: "Estado" }

        ComboBox {
            id: comboBox
            textRole: "text"
            valueRole: "value"
            model: [
                {text: "Cuentas pagadas", value: Filter.Status.Paid},
                {text: "Cuentas por pagar", value: Filter.Status.Unpaid},
                {text: "Cuentas abonadas", value: Filter.Status.Credited},
                {text: "Todas", value: Filter.Status.Any}
            ]
            Layout.fillWidth: true
            flat: true
            currentIndex: comboBox.count - 1
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
                const opt = comboBox.currentValue;
                // Asking for the same filter, do nothing & return.
                if (opt === _.previousFilter)
                    return;

                if (opt === Filter.Status.Any)
                    filtered(customerModel.model);
                else
                    filterOnStatus(opt);
                // Previous filter's option to know if it's asking ror the same
                _.previousFilter = opt;
            }
            Layout.alignment: Qt.AlignRight
        }
    }

    ListModel { id: filteredData }

    function filterOnStatus(status) {
        print("filterOnUnpaid: " + status);

        if (filteredData.count)
            filteredData.clear();

        for (let i = 0; i < customerModel.model.count; ++i) {
            const customer = customerModel.model.get(i);
            let filteredCustomer = {"orders": []};
            //find(p => p.status === 0, customer.orders);
            for (let j = 0; j <customer.orders.count; ++j) {
                const order = customer.orders.get(j);
                const payment = order.payment;


                if (payment.status === status) {
                    filteredCustomer.orders.push(order);
                    Object.assign(filteredCustomer,  {"person_info": customer.person_info})
                }
            }
            if (filteredCustomer.orders.length)
                filteredData.append(filteredCustomer);
        }
        filtered(filteredData);
    }
}

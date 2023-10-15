import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/singleton"
import "qrc:/utils"
import "qrc:/customers"
import core

ListView {
    id: root

    delegate: BillDelegate {
        required name
        required property int customer_id

        customerId: customer_id
    }

    model: CustomerManager.model

    Connections {
        target: CurrentUser

        function onUserChanged() {
            if (CustomerManager.modelIsEmpty()) customerAPI.fetchCustomers()
        }
    }

    Label {
        id: label
        anchors.centerIn: parent
        visible: count === 0
        width: root.width*.9
        text: "No hay ningun cliente para mostrar."
        font.pointSize: 16
        horizontalAlignment: Label.AlignHCenter
        wrapMode: Label.WordWrap
    }

    CustomerAPI {
        id: customerAPI
        onCustomersFetched: customers => CustomerManager.addCustomer(customers)
    }
}

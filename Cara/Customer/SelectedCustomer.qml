import QtQuick
import Cara.Controls
import Cara.Customer

Item {
    id: root

    property var customer

    signal changeCustomerClicked
    signal customerClicked(customer: var)

    implicitHeight: 150

    RedirectGroupBox {
        id: groupBox
        anchors.fill: parent
        title: "Cliente seleccionado"
        onRedirectClicked: root.changeCustomerClicked()

        CustomerCard {
            id: customerDelegate
            anchors.fill: parent
            customerId: internal.customerId
            fullname: internal.fullname
            onClicked: root.customerClicked(root.customer)
        }
    }

    QtObject {
        id: internal
        readonly property string fullname: root.customer ? root.customer.fullname : ""
        readonly property int customerId: root.customer ? root.customer.customerId : -1
    }

    function toJson() {
        const fullname = root.customer.fullname;
        const id = root.customer.customerId;
        return { fullname, id };
    }
}

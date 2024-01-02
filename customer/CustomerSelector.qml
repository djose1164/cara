import QtQuick
import QtQuick.Controls
import Utils
import Customer

Drawer {
    id: root

    required property int salespersonId
    readonly property alias model: customerModel
    property var selectedCustomer

    signal customerSelected(customer: var)

    interactive: false
    Component.onCompleted: {
        customerModel.update(salespersonId);
    }

    DrawerHeader {
        id: header
        anchors.top: parent.top
        width: root.width
        title: "Seleccionar cliente"
    }

    ListView {
        id: listView
        width: root.width
        anchors {
            top: header.bottom
            //topMargin: 15
            bottom: parent.bottom
            bottomMargin: 15
        }
        model: customerModel
        delegate: CustomerSelectorDelegate {
            required property var model
            required fullname
            required customerId

            width: listView.width
            height: 50
            onClicked: {
                customerSelected(model);
                root.close();
            }
        }

        BusyIndicator {
            id: busyIndicator
            anchors.centerIn: parent
        }

        Label {
            width: listView.width
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Label.AlignHCenter
            visible: listView.count === 0 && busyIndicator.visible === false
            text: "Aun no tienes ningun cliente registrado"
        }
    }

    CustomerModel {
        id: customerModel
        repository: customerRepository
        onUpdated: {
            busyIndicator.visible = false;
        }
    }

    RestCustomerRepository {
        id: customerRepository
    }
}

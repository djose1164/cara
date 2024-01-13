import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Customer
import Utils
import "qrc:/utils"

Item {
    id: root

    readonly property alias model: customerModel
    property alias searchText: customerProxy.text

    signal customerSelected(customer: var)
    signal customerClicked(customerId: int)

    ListView {
        id: listView
        anchors.fill: parent
        delegate: CustomerCard {
            required property var model
            required fullname
            required customerId

            width: listView.width
            height: 75
            onClicked: {
                customerSelected(model);
                customerClicked(customerId);
            }
        }
        model: customerProxy
        spacing: 20
        clip: true

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

    CustomerProxyModel {
        id: customerProxy
        sourceModel: customerModel
    }

    RestCustomerRepository {
        id: customerRepository
    }

    function updateModel(salespersonId: int) {
        customerModel.update(salespersonId);
    }
}

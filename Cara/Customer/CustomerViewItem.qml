import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cara.Customer
import Cara.Utils

Item {
    id: root

    required property CustomerModel customerModel
    property bool searchEnabled: CustomerManager.state !== "fetching"

    property alias searchText: customerProxy.text

    signal customerSelected(customer: var)
    signal customerClicked(customerId: int)

    states: [
        State {
            name: "contentNotFound"
            when: CustomerManager.state === "contentNotFound"
            PropertyChanges {
                target: errorPane
                visible: true
            }
        }

    ]

    //customerModel.onUpdated: busyIndicator.visible = false;
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
            running: CustomerManager.state === "fetching"
        }
    }

    MessagePane {
        id: errorPane
        visible: false
        width: root.width * .8
        height: root.height/2
        anchors.centerIn: parent
        text: "Aun no tienes ningun cliente registrado"
    }

    CustomerProxyModel {
        id: customerProxy
        sourceModel: customerModel
    }
}

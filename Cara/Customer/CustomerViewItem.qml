pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import Cara.Customer
import Cara.Controls

Item {
    id: root

    required property CustomerModel customerModel
    property bool searchEnabled: CustomerManager.state !== "fetching"

    property alias searchText: customerProxy.text
    property Component header
    property alias headerItem: headerLoader.item

    signal customerSelected(customer: var)
    signal customerClicked(customerId: int)

    states: [
        State {
            name: "contentNotFound"
            when: CustomerManager.state === "contentNotFound"
            PropertyChanges {
                errorPane.visible: true
            }
        },
        State {
            when: CustomerManager.state === "errorOccurred"
            PropertyChanges {
                errorPane.visible: true
                errorPane.text: CustomerManager.errorString
            }
        }
    ]

    Loader {
        id: headerLoader
        height: ApplicationWindow.window?.height * .12
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        sourceComponent: root.header
        z: 100
    }

    ListView {
        id: listView
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: headerLoader.status !== Loader.Null ? headerLoader.bottom : parent.top
        anchors.bottom: parent.bottom
        delegate: CustomerCard {
            required property var model
            required fullname
            required customerId

            width: listView.width
            height: 75
            onClicked: {
                root.customerSelected(model);
                root.customerClicked(customerId);
            }
        }
        model: customerProxy
        spacing: 20
        headerPositioning: ListView.PullBackHeader

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
        sourceModel: root.customerModel
    }
}

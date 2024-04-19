import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cara.Utils

Drawer {
    id: root

    readonly property alias model: customerView.customerModel

    signal customerSelected(customer: var)

    interactive: false

    Component.onCompleted: {
        headerItem.closeClicked.connect(root.close);
    }

    SearchHeaderItem {
        id: headerItem
        anchors.top: parent.top
        width: root.width
        height: 50
        title: "Seleccionar cliente"
    }

    CustomerViewItem {
        id: customerView
        width: root.width
        anchors.top: headerItem.bottom
        anchors.bottom: parent.bottom
        searchText: headerItem.displayText
        customerModel: CustomerManager.model
        header: headerItem
        onCustomerSelected: customer => {
            root.customerSelected(customer);
            root.close();
        }
    }
}

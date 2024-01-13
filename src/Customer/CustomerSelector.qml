import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Utils
import "qrc:/utils"

Drawer {
    id: root

    required property int salespersonId
    readonly property alias model: customerView.model

    signal customerSelected(customer: var)

    interactive: false

    Component.onCompleted: {
        customerView.updateModel(salespersonId);
        header.closeClicked.connect(root.close);
    }

    SearchHeaderItem {
        id: header
        anchors.top: parent.top
        width: root.width
        height: 50
        title: "Seleccionar cliente"
    }

    CustomerViewItem {
        id: customerView
        anchors.top: header.bottom
        anchors.bottom: parent.bottom
        width: root.width
        searchText: header.displayText
        onCustomerSelected: customer => {
            root.customerSelected(customer);
            root.close();
        }
    }
}

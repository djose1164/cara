import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cara.Utils

Drawer {
    id: root

    readonly property alias model: customerView.customerModel

    signal customerSelected(customer: var)

    enum View {
        TitleView,
        SearchView
    }

    interactive: false

    SelectorHeaderItem {
        id: searchHeader
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        title: "Seleccionar cliente"
        onCloseClicked: root.close()
    }

    CustomerViewItem {
        id: customerView
        anchors {
            top: searchHeader.bottom
            topMargin: 20
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        searchText: searchHeader.searchText
        customerModel: CustomerManager.model
        Layout.fillHeight: true
        onCustomerSelected: customer => {
            root.customerSelected(customer);
            root.close();
        }
    }
}

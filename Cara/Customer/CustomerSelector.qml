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

    CustomerViewItem {
        id: customerView
        anchors.fill: parent
        header: searchHeader
        searchText: (headerItem as SelectorHeaderItem).searchText
        customerModel: CustomerManager.model
        Layout.fillHeight: true
        onCustomerSelected: customer => {
            root.customerSelected(customer);
            root.close();
        }
    }

    Component {
        id: searchHeader
        SelectorHeaderItem {
            width: customerView.width
            z: 2
            title: "Seleccionar cliente"
            onCloseClicked: root.close()
        }
    }
}

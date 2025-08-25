import QtQuick
import QtQuick.Layouts
import Cara.Controls

Item {
    id: root

    enum View {
        Empty,
        Selected
    }

    signal selectedCustomerClicked(customer: var)

    implicitWidth: stackLayout.implicitWidth
    implicitHeight: internal.hasSelection ? stackLayout.implicitHeight
                                          : customerRedirect.implicitHeight

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        currentIndex: internal.hasSelection ? ChooseCustomer.Selected : ChooseCustomer.Empty

        RedirectLabel {
            id: customerRedirect
            title: "Elegir cliente"
            onClicked: customerSelector.open()
        }

        SelectedCustomer {
            id: selectedCustomer
            onChangeCustomerClicked: customerSelector.open()
            onCustomerClicked: c => root.selectedCustomerClicked(c)
        }
    }

    CustomerSelector {
        id: customerSelector
        width: app.width
        height: app.height
        onCustomerSelected: customer => {
            selectedCustomer.customer = customer;
            internal.hasSelection = true;
        }
    }

    QtObject {
        id: internal
        property bool hasSelection
    }

    function toJson() {
        return selectedCustomer.toJson();
    }

    function validate() {
        if (!internal.hasSelection)
            throw "Necesitas elegir un cliente";
    }
}

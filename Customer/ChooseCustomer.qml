import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Utils

Item {
    id: root

    enum View {
        Empty,
        Selected
    }

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
            font.pointSize: 14
            onClicked: customerSelector.open()
        }

        SelectedCustomer {
            id: selectedCustomer
            onChangeCustomerClicked: customerSelector.open()
        }
    }

    CustomerSelector {
        id: customerSelector
        width: app.width
        height: app.height
        onCustomerSelected: customer => {
            selectedCustomer.customerId = customer.customerId;
            selectedCustomer.fullname = customer.fullname;
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

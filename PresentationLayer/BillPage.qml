import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.cara 1.0
import "qrc:/utils"

Page {
    id: root

    readonly property StackView stackView: StackView.view

    title: "Facturas"
    header: BillPageHeader {
        stackView: root.stackView
        onOpenFilter: filter.open()
        //customerModel: baseModel
    }

    Bills {
        id: bills
        stackView: root.stackView
    }

    Filter {
        id: filter
        //customerModel: baseModel
        onFiltered: data => orderView.model = data
    }

    function updateCustomerModel() {
        if (TheCurrentUser.isAdmin)
            bills.findAll();
        else if (TheCurrentUser.isCostumer)
            bills.findOrdersByCustomerId(TheCurrentUser.customerId);
    }
}

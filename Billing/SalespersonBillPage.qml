import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import Utils
import Customer

Page {
    id: root

    readonly property T.StackView stackView: StackView.view

    SearchHeaderItem {
        id: headerItem
        anchors.top: parent.top
        width: root.width
        height: 50
        title: "Seleccionar cliente"
        closeBtnVisible: false
    }

    CustomerViewItem {
        id: cvItem
        anchors.top: headerItem.bottom
        anchors.bottom: parent.bottom
        width: root.width
        searchText: headerItem.displayText
        customerModel: CustomerManager.model
        header: headerItem
        onCustomerClicked: customerId => root.stackView.pushItem("qrc:/qt/qml/Billing/BillHomePage.qml", {customerId})
    }
}

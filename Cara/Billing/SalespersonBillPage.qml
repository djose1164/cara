import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import Cara.Controls
import Cara.Customer
import Cara.Salesperson
import Cara.Utils

Page {
    id: root

    readonly property T.StackView stackView: StackView.view

    enum View {
        TitleView,
        SearchView
    }

    header: StackLayout {
        id: stackLayout
        implicitHeight: 50
        enabled: cvItem.searchEnabled

        ToolBar {
            RowLayout {
                anchors.fill: parent
                spacing: 10

                Item {}

                Label {
                    text: "Seleccionar cliente"
                    rightPadding: 16
                    font.pointSize: 16
                    font.bold: true
                    Layout.fillWidth: true
                }

                SearchButton {
                    Layout.alignment: Qt.AlignRight
                    onClicked: stackLayout.currentIndex = CustomerSelector.SearchView
                }
            }
        }

        SearchItem {
            id: headerItem
            onSearchClosed: stackLayout.currentIndex = CustomerSelector.TitleView
        }
    }

    Component.onCompleted: {
        CustomerManager.populate(SalespersonSettings.salespersonId);
    }

    CustomerViewItem {
        id: cvItem
        anchors.fill: parent
        searchText: headerItem.displayText
        customerModel: CustomerManager.model
        onCustomerClicked: customerId => root.stackView.pushItem("qrc:/qt/qml/Cara/Billing/BillHomePage.qml", {customerId})
    }
}

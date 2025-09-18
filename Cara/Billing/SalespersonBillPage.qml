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

                MoreVertButton {
                    onClicked: menu.open()
                    Menu {
                        id: menu

                        MenuItem {
                            text: qsTr("Cuentas pendientes")
                            onClicked: root.stackView.pushItem("qrc:/qt/qml/Cara/Customer/CustomerOutstandingOrders.qml")
                        }
                    }
                }
            }
        }

        SearchItem {
            id: headerItem
            onSearchClosed: stackLayout.currentIndex = CustomerSelector.TitleView
        }
    }

    CustomerViewItem {
        id: cvItem
        anchors.fill: parent
        searchText: headerItem.displayText
        customerModel: CustomerManager.model
        onCustomerClicked: customerId => root.stackView.pushItem("qrc:/qt/qml/Cara/Billing/BillHomePage.qml", {customerId})
    }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import Cara
import Cara.Customer
import Cara.Salesperson
import Cara.User
import Cara.Utils

ListViewPage {
    id: root

    required property int salespersonId

    readonly property int fontSize: 12
    readonly property T.StackView stackView: StackView.view

    enum StackLayoutView {
        TitleView,
        SearchView
    }

    title: "Clientes"
    header: StackLayout {
        id: stackLayout

        TitleHeader {
            title: root.title
            btnLoader.sourceComponent: SearchButton {
                enabled: root.state !== "fetching"
                onClicked: stackLayout.currentIndex = Customers.SearchView
            }
            onBackClicked: stackView.popCurrentItem()
        }

        Rectangle {
            color: Material.backgroundColor

            SearchItem {
                id: searchItem
                anchors.fill: parent
                onSearchClosed: stackLayout.currentIndex = Customers.TitleView
            }
        }
    }
    noDataMessage: UserSettings.userTypeId === UserEntity.Admin
                   ? "Este vendedor no cuenta con ningun cliente."
                   : "La lista de clientes esta vacia. Inteta agregar uno nuevo."

    listView {
        anchors.topMargin: 5
        model: proxyModel
        spacing: 10
        delegate: CustomerCard {
            required fullname
            required customerId
            required property var model

            width: listView.width
            onClicked: {
                stackView.pushItem("qrc:/qt/qml/Cara/Customer/CustomerPage.qml", {customerId, model});
            }
        }
    }

    Component.onCompleted: {
        customerRepository.readBySalespersonId(salespersonId);
        root.state = "fetching";
    }

    CustomerProxyModel {
        id: proxyModel
        sourceModel: customerModel
        text: searchItem.displayText
    }

    CustomerModel {
        id: customerModel
        onUpdated: root.state = ""
    }

    RestService {
        CustomerResource {
            id: customerRepository
            path: "customers"
            onFetched: resp => customerModel.set(JSON.parse(resp).customers)
            onErrorStringChanged: print(errorString)
            onContentNotFound: root.state = "contentNotFound"
        }
    }

    RoundButton {
        anchors {
            right: parent.right
            rightMargin: 10
            bottom: parent.bottom
            bottomMargin: 15
        }
        icon.source: "qrc:/icons/person_add.svg"
        icon.width: 30
        icon.height: 30
        onClicked: stackView.push("qrc:qt/qml/Cara/Customer/CustomerForm.qml")
    }
}

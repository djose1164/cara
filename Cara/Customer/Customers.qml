import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import Cara.Salesperson
import Cara.User
import Cara.Utils
import Cara.Customer

ListViewPage {
    id: root

    required property int salespersonId

    readonly property int fontSize: 12
    readonly property T.StackView stackView: StackView.view

    title: "Clientes"
    header: TitleHeader {
        title: root.title
        btnLoader.sourceComponent: SearchButton {
            onClicked: root.state = "searching";
        }
        onBackClicked: stackView.pop()
    }
    noDataMessage: UserSettings.userTypeId === UserEntity.Admin
                   ? "Este vendedor no cuenta con ningun cliente."
                   : "La lista de clientes esta vacia. Inteta agregar uno nuevo."

    states: State {
        name: "searching"
        PropertyChanges {
            target: root
            header: null
        }
        PropertyChanges {
            target: listView
            anchors.topMargin: searchHeader.height * 1.05
        }
        PropertyChanges {
            target: searchHeader
            visible: true
            state: "searching"
        }
    }

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

    SearchHeaderItem {
        id: searchHeader
        anchors.top: parent.top
        width: root.width
        visible: false
        title: "Nombre"
        onSearchClosed: root.state = ""
    }

    CustomerProxyModel {
        id: proxyModel
        sourceModel: customerModel
        text: searchHeader.displayText
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
        onClicked: stackView.push("qrc:qt/qml/Customer/CustomerForm.qml")
    }
}

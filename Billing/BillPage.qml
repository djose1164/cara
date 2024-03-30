import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import Customer
import Utils
import User

Page {
    id: root

    required property var user
    readonly property T.StackView stackView: StackView.view
    readonly property string adminViewPath: "qrc:/qt/qml/Customer/CustomerViewItem.qml"
    readonly property string customerViewPath: "qrc:/qt/qml/Billing/BillHomePage.qml"
    readonly property string messagePath: "qrc:/qt/qml/Billing/BillMessage.qml"

    title: "Facturas"
    header: pageHeader

    states: State {
        when: user.isAdmin || user.isSalesperson
        name: "otherThanAdmin"
        PropertyChanges {
            target: root
            header: null
        }
    }

    onUserChanged: {
        if (user.customerId > 0)
            loader.setSource(customerViewPath, {
                                 customerId: UserSettings.customerId,
                                 header: null
                             });
        else if (user.isAdmin || user.isSalesperson)
            loader.sourceComponent = customerViewItem;
        else
            loader.setSource(messagePath, {user});
    }

    BillPageHeader {
        id: pageHeader
        stackView: root.stackView
        buttonLoader.sourceComponent: moreVertBtn
        onDoSearch: billStack.push("qrc:/qt/qml/Billing/BillSearch.qml", {model: loader.item.model, user});
    }

    Component {
        id: moreVertBtn

        MoreVertButton {
            onClicked: menu.open()

            Menu {
                id: menu

                MenuItem {
                    text: "Filtros"
                }

                MenuItem {
                    text: "Cambiar vista"
                    onTriggered: billViewChooser.open()
                }
            }
        }
    }

    BillViewChooser {
        id: billViewChooser
        width: root.width*.8
        height: root.height/2
        onViewChanged: newView => print("Requested view: " + newView)
    }

    Loader {
        id: loader
        readonly property int customerId: user.customerId
        anchors.fill: parent
    }

    Component {
        id: customerViewItem
        Item {
            id: itemParent

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
                onCustomerClicked: customerId => billStack.push("qrc:/qt/qml/Billing/BillHomePage.qml", {customerId})
            }
        }
    }
}

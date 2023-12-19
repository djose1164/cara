import QtQuick
import QtQuick.Controls
import "qrc:/utils"

Page {
    id: root

    required property var user
    readonly property StackView stackView: StackView.view
    readonly property string adminViewPath: "qrc:/bills/CustomersView.qml"
    readonly property string customerViewPath: "qrc:/bills/CustomerBillsHome.qml"
    readonly property string messagePath: "qrc:/bills/BillMessage.qml"

    title: "Facturas"
    header: pageHeader
    onUserChanged: {
        if (user.customerId > 0)
            loader.setSource(customerViewPath);
        else if (user.isAdmin || user.isSalesperson)
            loader.setSource(adminViewPath);
        else
            loader.setSource(messagePath, {user});
    }

    BillPageHeader {
        id: pageHeader
        stackView: root.stackView
        buttonLoader.sourceComponent: moreVertBtn
        onDoSearch: billStack.push("qrc:/bills/BillSearch.qml", {model: loader.item.model});
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
        onSourceChanged: print("Loader.onSourceChanged: "+source)
    }
}

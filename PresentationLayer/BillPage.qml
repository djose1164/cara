import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.cara 1.0
import "qrc:/singleton"
import "qrc:/utils"

Page {
    id: root

    readonly property StackView stackView: StackView.view
    readonly property UserEntity user: CurrentUser.user
    readonly property string adminViewPath: "qrc:/bills/CustomersView.qml"
    readonly property string messagePath: "qrc:/bills/BillMessage.qml"

    title: "Facturas"
    header: pageHeader

    BillPageHeader {
        id: pageHeader
        stackView: root.stackView
        buttonLoader.source: "qrc:/bills/BillPageButtons.qml"
        //customerModel: baseModel

        Connections {
            target: pageHeader.buttonLoader.item

            function onOpenFilter() {
                filter.open();
            }

            function onDoSearch() {
                billStack.push("qrc:/bills/BillSearch.qml", {model: loader.model});
            }
        }
    }

    states: [
        State {
            name: "userNotLoggedIn"
            when: user.userType === UserEntity.GUEST
            PropertyChanges {
                target: loader
                source: messagePath
            }
        },
        State {
            name: "userLoggedIn"
            when: user.userType === UserEntity.USER && CurrentUser.accessToken
            PropertyChanges {
                target: loader
                source: user.customerId ? adminViewPath : messagePath
            }
        },
        State {
            name: "adminLoggedIn"
            when: CurrentUser.isAdmin && CurrentUser.accessToken !== ""
            PropertyChanges {
                target: loader
                source: adminViewPath
            }
        }
    ]

    Loader {
        id: loader
        property ListModel stackView: root.stackView
        readonly property ListModel model: item.model
        anchors.fill: parent
    }

    Connections {
        target: LoginManager

        function onLoggingOut() {
            const loadedItem = loader.item;
            print("---Bills: loggingOut");
            item.clearModel();
        }
    }


    Filter {
        id: filter
        customerModel: loader.item.model
        onFiltered: data => orderView.model = data
    }

    Connections {
        target: LoginManager

        function onLoggedOut() {
            CustomerManager.clearModel();
        }
    }

    function updateCustomerModel() {
        if (TheCurrentUser.isAdmin)
            bills.findAll();
        else if (TheCurrentUser.isCostumer)
            bills.findOrdersByCustomerId(TheCurrentUser.customerId);
    }
}

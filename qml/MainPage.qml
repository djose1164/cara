import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import core
import User
import Salesperson
import "qrc:/orders" as Orders
import "qrc:/singleton"
import "qrc:/bills"

// TODO: MaiPage should be deleted in a future release.
// Plan in what release it should be deleted.
Page {
    id: root

    required property var user

    readonly property alias billStack: billStack
    // readonly property alias homeStack: homeStack

    readonly property StackView stackView: StackView.view
    readonly property string userHomePath: "qrc:/home/UserHome.qml"
    readonly property url adminHomePath: "qrc:/home/AdminHome.qml"
    readonly property url adminCartPath: "qrc:/orders/OrderFormAdmin.qml"
    readonly property url userCartPath: "qrc:/cart/Cart.qml"
    readonly property bool canPosteOrder: isAdmin || isSalesperson
    readonly property bool isAdmin: user.isAdmin
    readonly property bool isSalesperson: user.isSalesperson
    readonly property string username: user.username
    readonly property int userType: user.userType

    signal adminChanged

    footer: MainFooter {
        id: bar

        property int clickedCount
        property bool wasHomeLastView

        onHomeClicked: {
            ++clickedCount;

            if (homeStack.isCurrentView && !wasHomeLastView) {
                homeStack.pop(null);
            }
            else if (clickedCount == 2) {
                homeStack.pop(null);
                clickedCount = 0;
            }
        }

        onCurrentIndexChanged: {
            if (homeStack.isCurrentView)
                wasHomeLastView = false;
            else {
                wasHomeLastView = true;
                clickedCount = 0;
            }
        }
    }

    states: [
        State {
            name: "fetching"
            PropertyChanges {
                target: stackLayout
                visible: false
            }
            PropertyChanges {
                target: busyIndicator
                visible: true
            }
        }
    ]

    Component.onCompleted: {
        updateViews();
    }

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        currentIndex: bar.currentIndex

        StackView {
            id: homeStack
            readonly property TabBar bar: bar
            readonly property bool isCurrentView: StackLayout.isCurrentItem
        }

        StackView {
            id: orderStackView
        }

        StackView {
            id: billStack
            Component.onCompleted: replace("qrc:/bills/BillPage.qml", {user})
        }
    }


    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        visible: false
    }

    SidePane {
        id: sideMenu
        stackView: root.stackView
        user: root.user
    }

    Connections {
        target: LoginManager

        function onLoggedIn() {
            userStore.update();
            userController.getUserById(userStore.user.userId);
            root.state = "fetching";
        }

        function onLoggingOut() {
            CustomerManager.clearModel();
            root.user = userController.create({});
        }

        function onLoggedOut() {
            updateViews();
        }
    }

    LocalUserStore { id: userStore }

    UserController {
        id: userController
        onFetched: function(data) {
            root.user = create(
                JSON.stringify(
                    JSON.parse(data).user
            ));
            updateViews();
            root.state = "";
        }
    }

    AssociateSalespersonService {
        id: spService
        salespersonId: user.salespersonId
        onFetched: CustomerManager.addCustomer(JSON.parse(data).customers)
    }

    function updateViews(): void {
        switch (userType) {
        case UserEntity.Admin:
        case UserEntity.Salesperson:
            if (CustomerManager.isEmpty) spService.fetchCustomers();
            homeStack.replace(adminHomePath);
            orderStackView.replace(adminCartPath, {salesperson: user.salesperson});
            break;
        case UserEntity.Guest:
        case UserEntity.Customer:
        case UserEntity.User:
            homeStack.replace(userHomePath);
            orderStackView.replace(userCartPath);
            break;
        }
    }
}

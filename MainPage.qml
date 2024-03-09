import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import User
import Salesperson
import "qrc:/orders" as Orders
import Customer
import Billing
import Authentication

// TODO: MaiPage should be deleted in a future release.
// Plan in what release it should be deleted.
Page {
    id: root

    required property var user

    readonly property alias billStack: billStack

    readonly property T.StackView currentStackView: stackLayout.children[stackLayout.currentIndex]
    readonly property StackView stackView: StackView.view
    readonly property string userHomePath: "qrc:/home/UserHome.qml"
    readonly property url adminHomePath: "qrc:/home/AdminHome.qml"
    readonly property url adminCartPath: "qrc:/qt/qml/Billing/NewPurchaseOrder.qml"
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
            Component.onCompleted: replace("qrc:/qt/qml/Billing/BillPage.qml", {user});
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

    Connections {
        target: Application

        function onStateChanged(state: int) {
            if (state === Qt.ApplicationActive && !TokenManager.refreshing)
                TokenManager.resumeRefreshing();
            else if (state === Qt.ApplicationInactive && TokenManager.refreshing)
                TokenManager.stopRefreshing();
            print(`onApplicationStateChanged: ${state}`);
        }
    }

    UserController {
        id: userController
        onFetched: function(data) {
            root.user = userController.create(
                JSON.stringify(
                    JSON.parse(data).user
            ));
            updateViews();
            root.state = "";
        }
    }

    AssociateSalespersonService {
        id: spService
        salespersonId: SalespersonSettings.salespersonId
    }

    function updateViews(): void {
        switch (userType) {
        case UserEntity.Admin:
        case UserEntity.Salesperson:
            CustomerManager.populate(SalespersonSettings.salespersonId);

            if (CustomerManager.isEmpty) spService.fetchCustomers();
            homeStack.replace(adminHomePath);
            orderStackView.replace(adminCartPath/*, {salesperson: user.salesperson}*/);
            break;
        case UserEntity.Guest:
        case UserEntity.Customer:
        case UserEntity.User:
            homeStack.replace(userHomePath);
            orderStackView.replace(userCartPath);
            break;
        }
        if (billStack.currentItem?.user)
            billStack.currentItem.user = root.user;
    }
}

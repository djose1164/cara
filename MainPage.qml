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
import QtNetwork

// TODO: MaiPage should be deleted in a future release.
// Plan in what release it should be deleted.
Page {
    id: root

    readonly property var user: UserSettings.entity()

    readonly property alias billStack: billStack

    readonly property T.StackView currentStackView: stackLayout.children[stackLayout.currentIndex]
    readonly property T.StackView stackView: StackView.view
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
            when: NetworkInformation.reachability !== NetworkInformation.Reachability.Online
            PropertyChanges {
                target: offlinePane
                visible: true
            }
            PropertyChanges {
                target: root
                enabled: false
            }
            PropertyChanges {
                target: stackLayout
                visible: false
            }
        }
    ]

    Component.onCompleted: {
        updateViews();
    }

    Pane {
        id: offlinePane
        width: root.width * .9
        anchors.centerIn: parent
        visible: false

        ColumnLayout {
            anchors.fill: parent
            spacing: 10

            Image {
                source: "wifi-off.svg"
                sourceSize: "32x32"
                Layout.alignment: Qt.AlignHCenter
                Layout.bottomMargin: 10
            }

            Label {
                text: "No estas conectado a Intenet"
                font.pointSize: 16
                font.bold: true
                Layout.fillWidth: true
            }

            Label {
                text: "Verifica tu conexion de internet. Intenta verificar que"
                      + " tu WiFi o datos mobiles esten activiados."
                font.pointSize: 14
                wrapMode: Label.Wrap
                Layout.fillWidth: true
            }
        }
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
        }
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        visible: false
    }

    SidePane {
        id: sideMenu
        implicitWidth: root.width
        implicitHeight: root.height
        stackView: root.stackView
    }

    Connections {
        target: LoginManager

        function onLoggedIn() {
            updateViews();
        }

        function onLoggingOut() {
            CustomerManager.clearModel();
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

    SalespersonController {
        id: spService
        salespersonId: SalespersonSettings.salespersonId
    }

    QtObject {
        id: internal
        readonly property url customerBillPage: "qrc:/qt/qml/Billing/BillHomePage.qml"
        readonly property url salespersonBillPage: "qrc:/qt/qml/Billing/SalespersonBillPage.qml"
        readonly property url billMessage: "qrc:/qt/qml/Billing/BillMessage.qml"
    }

    function updateViews(): void {
        switch (userType) {
        case UserEntity.Admin:
        case UserEntity.Salesperson:
            CustomerManager.populate(SalespersonSettings.salespersonId);
            if (CustomerManager.isEmpty) spService.fetchCustomers();
            homeStack.replace(adminHomePath);
            orderStackView.replace(adminCartPath);
            billStack.replace(internal.salespersonBillPage)
            break;
        case UserEntity.Guest:
            homeStack.replace(userHomePath);
            orderStackView.replace(userCartPath);
            billStack.replaceCurrentItem(internal.billMessage)
            break;
        case UserEntity.Customer:
        case UserEntity.User:
            console.log("Updating views to customer/user...")
            homeStack.replace(userHomePath);
            orderStackView.replace(userCartPath);
            billStack.replaceCurrentItem(internal.customerBillPage, {customerId: UserSettings.customerId})
            break;
        }
    }
}

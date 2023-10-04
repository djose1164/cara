import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import core
import "qrc:/orders" as Orders
import "qrc:/singleton"

Page {
    id: root

    readonly property alias billStack: billStack

    readonly property var stackView: StackView.view
    readonly property Component busyIndicator: BusyIndicator {}
    readonly property bool isAdmin: CurrentUser.isAdmin
    readonly property string homePath: isAdmin ? "qrc:/home/AdminHome.qml"
                                               : "qrc:/home/UserHome.qml"
    readonly property string cartPath: isAdmin ? "qrc:/orders/OrderFormAdmin.qml"
                                               : "qrc:/cart/Cart.qml"

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

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        currentIndex: bar.currentIndex

        StackView {
            id: homeStack
            readonly property TabBar bar: bar
            readonly property bool isCurrentView: StackLayout.isCurrentItem
            initialItem: homePath ?? busyIndicator
        }

        StackView {
            id: orderStackView
            initialItem: cartPath ?? busyIndicator
        }

        StackView {
            id: billStack
            initialItem: "qrc:/bills/BillPage.qml"
        }
    }

    SidePane {
        id: sideMenu
        stackView: root.stackView
    }

    Connections {
        target: CurrentUser

        function onIsAdminChanged() {
            homeStack.replace(homePath ?? busyIndicator);
            orderStackView.replace(cartPath ?? busyIndicator);
        }
    }
}

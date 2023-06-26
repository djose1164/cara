import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.cara
import "qrc:/orders" as Orders
import "qrc:/singleton"

Page {
    id: root
    readonly property StackView stackView: StackView.view
    readonly property Component busyIndicator: BusyIndicator {}
    readonly property bool isAdmin: CurrentUser.userType() === CurrentUser.ADMIN
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

            if (homeView.isCurrentView && !wasHomeLastView) {
                homeView.pop(null);
            }
            else if (clickedCount == 2) {
                homeView.pop(null);
                clickedCount = 0;
            }
        }

        onCurrentIndexChanged: {
            if (homeView.isCurrentView)
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
            id: homeView
            readonly property TabBar bar: bar
            readonly property bool isCurrentView: StackLayout.isCurrentItem
            initialItem: homePath ?? busyIndicator
        }

        StackView {
            id: orderStackView
            initialItem: busyIndicator

            Connections {
                target: CurrentUser

                function onRestReady(token: string) {
                    CurrentUser.accessToken = token;
                    orderStackView.replace(cartPath);
                }
            }
        }

        StackView { initialItem: "qrc:/bills/BillPage.qml" }
    }

    SidePane {
        id: sideMenu
        stackView: root.stackView
    }
}

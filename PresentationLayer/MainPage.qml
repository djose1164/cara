import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/orders" as Orders
import "qrc:/singleton"

Page {
    id: root
    readonly property StackView stackView: StackView.view
    readonly property Component busyIndicator: BusyIndicator {}

    signal adminChanged
    footer: MainFooter {
        id: bar

        property int clickedCount
        property bool wasHomeLastView

        onHomeClicked: {
            ++clickedCount;

            if (homeView.isCurrentView && !wasHomeLastView) {
                print("## homeView is current view.")
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
            initialItem: TheCurrentUser.homePath ?? busyIndicator
        }

        StackView {
            id: orderStackView
            initialItem: TheCurrentUser.cartPath ?? busyIndicator

            Connections {
                target: TheCurrentUser

                function onCartPathChanged() {
                    orderStackView.replace(TheCurrentUser.cartPath);
                }
            }
        }

        StackView { initialItem: "qrc:/bills/BillPage.qml" }
    }

    SidePane {
        id: sideMenu
        stackView: root.stackView
    }

    Connections {
        target: TheCurrentUser

        function onUserCredentialsSet() {
            homeView.replace(TheCurrentUser.homePath);
        }
    }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/orders" as Orders
import "qrc:/users"

Page {
    id: root
    readonly property StackView stackView: StackView.view

    footer: MainFooter {id: bar }

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        currentIndex: bar.currentIndex

        StackView {
            id: homeView
            readonly property TabBar bar: bar
            initialItem: BusyIndicator {}   
        }

        StackView {
            id: orderStackView
            initialItem: BusyIndicator {}
        }

        StackView { initialItem: "qrc:/bills/BillPage.qml" }
    }

    Connections {
        target: TheCurrentUser
        function onUserCredentialsSet() {
            print("onUserCredentialsSet: " + TheCurrentUser.homePath)
            homeView.replace(TheCurrentUser.homePath)

            print("orderStackView pusing: " + TheCurrentUser.orderFormPath);
            orderStackView.push(TheCurrentUser.orderFormPath);
        }
    }

    Connections {
        target: bar

        function onHomeClicked() {
            for (let depth = homeView.depth; depth > 1; --depth)
                homeView.pop();
        }
    }

    SidePane { id: sideMenu }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/orders" as Orders
import "qrc:/users"

Page {
    id: root
    readonly property StackView stackView: StackView.view
    readonly property Component busyIndicator: BusyIndicator {}

    signal adminChanged
    footer: MainFooter {id: bar }

    Component.onCompleted: {
        //if (homeView.currentItem === busyIndicator)
        //    homeView.replace(TheCurrentUser.homePath)


        print("orderStackView pusing: " + TheCurrentUser.orderFormPath);
        print("homeStackView: " + TheCurrentUser.homePath);
        orderStackView.push(TheCurrentUser.orderFormPath);
    }

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        currentIndex: bar.currentIndex

        StackView {
            id: homeView
            readonly property TabBar bar: bar
            initialItem: TheCurrentUser.homePath ?? busyIndicator
        }

        StackView {
            id: orderStackView
            initialItem: TheCurrentUser.orderPath ?? busyIndicator
        }

        StackView { initialItem: "qrc:/bills/BillPage.qml" }
    }

    SidePane {
        id: sideMenu
        stackView: root.stackView
    }

    Connections {
        target: TheCurrentUser

        function onHomeChanged() {
            print("homeStackView: " + TheCurrentUser.homePath);
            homeView.replace(TheCurrentUser.homePath);
            const home = homeView.currentItem;
            bar.homeClicked.connect(home.homeClicked);
        }
    }
}

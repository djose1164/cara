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
            initialItem: TheCurrentUser.homePath
        }

        StackView {
            id: orderStackView
            initialItem: TheCurrentUser.orderPath
        }

        StackView { initialItem: "qrc:/bills/BillPage.qml" }
    }

    Connections {
        target: TheCurrentUser
        function onUserCredentialsSet() {
            print("onUserCredentialsSet: " + TheCurrentUser.homePath)
            homeView.replace(TheCurrentUser.homePath)
            const home = homeView.currentItem;
            bar.homeClicked.connect(home.homeClicked);

            print("orderStackView pusing: " + TheCurrentUser.orderFormPath);
            orderStackView.push(TheCurrentUser.orderFormPath);
        }
    }

    SidePane { id: sideMenu }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/orders" as Orders
import "qrc:/users"

Page {
    id: root
    readonly property StackView stackView: StackView.view

    //anchors.fill: parent
    footer: MainFooter { id: bar }

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
            homeView.push(TheCurrentUser.homePath)

            print("orderStackView pusing: " + TheCurrentUser.orderFormPath);
            orderStackView.push(TheCurrentUser.orderFormPath);
        }
    }

    SidePane { id: sideMenu }
}

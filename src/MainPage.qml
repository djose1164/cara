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
        onHomeClicked: homeView.pop(null)
    }

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        currentIndex: bar.currentIndex

        StackView {
            id: homeView
            readonly property TabBar bar: bar
            initialItem: TheCurrentUser.isGuest ? TheCurrentUser.homePath
                                                : busyIndicator
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

        function onUserCredentialsSet() {
            homeView.replace(TheCurrentUser.homePath);
            orderStackView.replace(TheCurrentUser.orderFormPath);
        }
    }
}

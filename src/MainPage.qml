import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/orders" as Orders

Page {
    id: root

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        currentIndex: bar.currentIndex

        StackView {
            id: homeStackView
            initialItem: "qrc:/home/Home.qml"
        }

        StackView {
            id: searchStackView
            initialItem: "qrc:/search/SearchPage.qml"
        }

        StackView {
            id: orderStackView

            readonly property bool currentView: StackLayout.isCurrentItem
            onCurrentViewChanged: if (currentView) addOrderPage.viewRequested()

            initialItem: Orders.AddOrderPage { id: addOrderPage }
        }

        StackView {
            id: billStackView
            initialItem: "qrc:/bills/BillPage.qml"
        }
    }

    footer: MainFooter { id: bar }

    SidePane {id: sideMenu}
}

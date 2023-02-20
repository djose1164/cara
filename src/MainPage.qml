import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/orders" as Orders
import "qrc:/users"

Page {
    id: root

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        currentIndex: bar.currentIndex

        onCurrentIndexChanged: {
            if (bar.addOrderIdx === currentIndex)
                orderStackView.currentItem.viewRequested();
        }

        StackView { initialItem: TheCurrentUser.homePath }

        StackView { initialItem: "qrc:/search/SearchPage.qml" }

        StackView {
            id: orderStackView
            initialItem: "qrc:/orders/AddOrderPage.qml"
        }

        StackView { initialItem: "qrc:/bills/BillPage.qml" }
    }

    footer: MainFooter { id: bar }

    SidePane {id: sideMenu}
}

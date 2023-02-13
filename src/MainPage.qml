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

        onCurrentIndexChanged: {
            if (bar.addOrderIdx === currentIndex)
                orderStackView.currentItem.viewRequested();
        }

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
            initialItem: "qrc:/orders/AddOrderPage.qml"
        }

        StackView {
            id: billStackView
            initialItem: "qrc:/bills/BillPage.qml"
        }
    }

    footer: MainFooter { id: bar }

    SidePane {id: sideMenu}
}

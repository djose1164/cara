import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root
    property alias mainStackLayout: stackLayout

    header: MainHeader { id: mainHeader }

    StackLayout {
        id: stackLayout

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

        onCurrentIndexChanged: mainHeader.currentIndexChanged(currentIndex)
    }

    footer: MainFooter {}

    SidePane {id: sideMenu}
}

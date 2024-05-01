import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cara.User

TabBar {
    id: root

    readonly property int addOrderIdx: shopBtn.TabBar.index

    signal homeClicked

    background: Rectangle {
        anchors.fill: parent
        color: "floralwhite"
    }

    TabButton {
        id: homeBtn
        icon.source: "qrc:/icons/home.svg"
        onClicked: homeClicked()
    }

    TabButton {
        id: shopBtn
        icon.source: "qrc:/icons/shopping-cart.svg"
    }

    TabButton {
        id: billBtn
        icon.source: "qrc:/icons/bill.svg"
    }
}

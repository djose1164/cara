import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

TabBar {
    readonly property int addOrderIdx: shopBtn.TabBar.index

    background: Rectangle {
        anchors.fill: parent
        color: "floralwhite"
    }

    TabButton {
        id: homeBtn
        background: IconImage {
            source: "qrc:/icons/home.svg"
            antialiasing: false
            sourceSize: iconSize
        }
    }

    TabButton {
        id: shopBtn
        background: IconImage {
            source: "qrc:/icons/shopping-cart.svg"
            antialiasing: false
            sourceSize: iconSize
        }
    }

    TabButton {
        id: billBtn
        background: IconImage {
            source: "qrc:/icons/bill.svg"
            antialiasing: false
            sourceSize: "32x32"
        }
    }
}

import QtQuick
import QtQuick.Controls

TabBar {
    id: root

    readonly property int addOrderIdx: shopBtn.TabBar.index
    property bool isAdminOrSeller

    signal homeClicked

    onIsAdminOrSellerChanged: {
        if (root.isAdminOrSeller && root.count === 3) {
            const btn = internal.createTabButton(customerBtn)
            root.addItem(btn)
        } else if (root.count > 3) {
            const btn = root.itemAt(root.count - 1)
            root.removeItem(btn)
        }
    }

    background: Rectangle {
        anchors.fill: parent
        color: "floralwhite"
    }

    TabButton {
        id: homeBtn
        icon.source: "qrc:/icons/home.svg"
        onClicked: root.homeClicked()
    }

    TabButton {
        id: shopBtn
        icon.source: "qrc:/icons/shopping-cart.svg"
    }

    TabButton {
        id: billBtn
        icon.source: "qrc:/icons/bill.svg"
    }

    Component {
        id: customerBtn
        TabButton {
            icon.source: "qrc:/qt/qml/Cara/Billing/assets/customer_icon.svg"
        }
    }

    QtObject {
        id: internal
        function createTabButton(button: Component): Item {
            return button.createObject(root) as Item;
        }
    }
}

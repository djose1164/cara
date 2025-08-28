pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import Cara.ProductInventory

Drawer {
    id: root

    property alias productModel: productView.model
    property alias delegate: productView.delegate
    property alias selectionModel: productView.selectionModel

    interactive: false

    ProductViewItem {
        id: productView
        anchors.fill: parent
        onCloseClicked: root.close()
    }
}

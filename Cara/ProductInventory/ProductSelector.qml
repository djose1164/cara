pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls

import Cara.ProductInventory

Drawer {
    id: root

    required property int salespersonId
    property alias selectedProducts: productView.selectedProducts
    property alias productModel: productView.model
    property alias delegate: productView.delegate
    readonly property alias selectionModel: productView.selectionModel

    signal productSelected(products: ProductModel)

    interactive: false

    ProductViewItem {
        id: productView
        anchors.fill: parent
        onCloseClicked: root.close()
    }

    function removeSelectionOf(productId) {
        productView.removeSelectionOf(productId);
    }

    function deselectAll() {
        productView.deselectAll();
    }
}

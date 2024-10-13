pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls

import Cara.ProductInventory

Drawer {
    id: root

    required property int salespersonId
    required property ProductModel selectedProducts
    property alias productModel: productView.model
    property alias delegate: productView.delegate
    property alias model: productView.model
    property alias selectionModel: productView.selectionModel

    signal productSelected(products: ProductModel)

    interactive: false

    ProductViewItem {
        id: productView
        anchors.fill: parent
        selectedProducts: root.selectedProducts
        onCloseClicked: root.close()
    }

    function removeSelectionOf(productId) {
        productView.removeSelectionOf(productId);
    }

    function deselectAll() {
        productView.deselectAll();
    }
}

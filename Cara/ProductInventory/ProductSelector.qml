import QtQuick
import QtQuick.Controls
import Cara.Utils
import Cara.ProductInventory

Drawer {
    id: root

    required property int salespersonId
    required property ProductModel selectedProducts
    property alias productModel: productView.productModel

    signal productSelected(products: ProductModel)

    interactive: false

    Component.onCompleted: {
        refresh();
    }

    SelectorHeaderItem {
        id: searchHeader
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        title: "Seleccionar producto"
        onCloseClicked: root.close()
    }

    ProductViewItem {
        id: productView
        width: root.width
        anchors.top: searchHeader.bottom
        anchors.bottom: parent.bottom
        selectedProducts: root.selectedProducts
        searchText: searchHeader.searchText
    }

    function removeSelectionOf(productId) {
        productView.removeSelectionOf(productId);
    }

    function deselectAll() {
        productView.deselectAll();
    }

    function refresh() {
        productView.updateModel(salespersonId);
    }
}

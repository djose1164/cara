import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQml.Models
import Cara.Utils
import Cara.Inventory
import Cara.Billing
import Cara.Product

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

    SearchHeaderItem {
        id: headerItem
        anchors.top: parent.top
        width: root.width
        height: 50
        title: "Seleccionar producto"
        onCloseClicked: root.close()
    }

    ProductViewItem {
        id: productView
        width: root.width
        anchors.top: headerItem.bottom
        anchors.bottom: parent.bottom
        selectedProducts: root.selectedProducts
        header: headerItem
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

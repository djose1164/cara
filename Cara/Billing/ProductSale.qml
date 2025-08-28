import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Cara.Controls
import Cara.ProductInventory
import Cara.Billing

Item {
    id: root

    enum View {
        Empty,
        Selecting
    }

    property alias calculator: costCalculator
    property alias selectorModel: productSelector.productModel
    property alias selectorDelegate: productSelector.delegate
    property alias selectedProductsDelegate: selectedProductsView.delegate
    readonly property alias implicitViewWidth: selectedProductsView.implicitViewWidth
    readonly property alias implicitViewHeight: selectedProductsView.implicitViewHeight
    readonly property alias selectedProductsView: selectedProductsView
    readonly property alias selectionModel: productSelector.selectionModel

    implicitWidth: stackLayout.implicitWidth
    implicitHeight: stackLayout.implicitHeight

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        currentIndex: internal.hasSelection ? ProductSale.Selecting : ProductSale.Empty
        implicitHeight: {
            if (currentIndex === ProductSale.Empty)
                return productRedirect.implicitHeight;
            const count = selectedProductsView.model.count;
            const itemHeight = 180;
            if (count === 1)
                return itemHeight;
            else if (count === 2)
                return itemHeight * 1.65;
            else
                return itemHeight * 2.1;
        }

        RedirectLabel {
            id: productRedirect
            title: "Agregar producto(s)"
            Layout.maximumHeight: implicitHeight
            onClicked: productSelector.open()
        }

        SelectedProducts {
            id: selectedProductsView
            model: productSelectionModel
            onRedirectClicked: productSelector.open()
            onDeleteClicked: (index, _) => {
                const proxyIdx = productSelectionModel.index(index,0);
                const idx = productSelectionModel.mapToSource(proxyIdx);
                itemSelectionModel.select(idx, ItemSelectionModel.Deselect)
            }
        }
    }

    ProductSelector {
        id: productSelector
        width: ApplicationWindow.window.width
        height: ApplicationWindow.window.height
        selectionModel: itemSelectionModel
    }

    SelectionProxyModel {
        id: productSelectionModel
        sourceModel: productSelector.productModel
        selectionModel: itemSelectionModel
    }

    ItemSelectionModel {
        id: itemSelectionModel
        model: root.selectorModel
    }

    CostCalculator {
        id: costCalculator
        model: productSelectionModel
    }

    QtObject {
        id: internal
        readonly property bool hasSelection: productSelectionModel.count > 0
    }

    function validate() {
        if (!internal.hasSelection)
            throw "Debes agregar al menos un producto a la orden";
    }

    function validateStocks() {
        const productModel = productSelector.productModel;
        for (let idx of itemSelectionModel.selectedIndexes) {
            root.enoughStocksFor(productModel.get(idx.row));
        }
    }

    function enoughStocksFor(product) {
        const model = productSelector.productModel;
        const foundIndex = model.find(product.productId);

        if (!foundIndex.valid)
            throw `Producto '${product.name}' no encontrado en la lista de seleccion.`;

        const foundProduct = model.get(foundIndex.row);
        print(`${foundProduct.quantityAvailable} < ${product.selectedQty}`)
        if (foundProduct.quantityAvailable < product.selectedQty)
            throw `No hay suficiente ${product.name} disponible en inventario.`;
    }

    function toJson() {
        console.log("ProductSale: ", JSON.stringify(productSelectionModel.toJsonArray(),null,2))
        return productSelectionModel.toJsonArray();
    }

    function refreshSelector() {
        itemSelectionModel.clear();
    }
}

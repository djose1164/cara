import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Utils
import Inventory
import Salesperson
import Billing

Item {
    id: root

    enum View {
        Empty,
        Selecting
    }

    property alias choosenProducts: selectedProductsView.model
    property alias calculator: costCalculator

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
            const itemHeight = 160;
            if (count === 1)
                return itemHeight;
            else if (count === 2)
                return itemHeight * 1.65;
            else
                return itemHeight * 2;
        }

        RedirectLabel {
            id: productRedirect
            title: "Agregar producto(s)"
            font.pointSize: 14
            Layout.maximumHeight: implicitHeight
            onClicked: productSelector.open()
        }

        SelectedProducts {
            id: selectedProductsView
            onRedirectClicked: productSelector.open()
            onDeleteClicked: (_, productId) => {
                productSelector.removeSelectionOf(productId);
            }
        }
    }

    ProductSelector {
        id: productSelector
        width: app.width
        height: app.height
        salespersonId: SalespersonSettings.salespersonId
        selectedProducts: selectedProductsView.model
    }

    CostCalculator {
        id: costCalculator
        model: selectedProductsView.model
    }

    QtObject {
        id: internal
        readonly property bool hasSelection: selectedProductsView.model.count > 0
    }

    function validate() {
        if (costCalculator.total === 0)
            throw "Debes agregar al menos un producto a la orden";
    }

    function toJson() {
        return selectedProductsView.toJson();
    }
}

pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQml.Models
import Cara.Product
import Cara.Controls
import Cara.ProductInventory

Item {
    id: root

    required property ProductModel selectedProducts
    required property string searchText
    readonly property ProductModel productModel: ProductInventoryController.inventory

    states: [
        State {
            name: "fetching"
            PropertyChanges {
                busyIndicator.running: true
            }
        },
        State {
            name: "empty"
            PropertyChanges {
                errorPane.visible: true
            }
        }
    ]

    ListView {
        id: listView
        anchors.fill: parent
        model: proxyModel
        delegate: productDelegate
        clip: true
        spacing: 10
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        running: false
    }

    MessagePane {
        id: errorPane
        visible: false
        width: root.width * .8
        height: root.height/2
        anchors.centerIn: parent
        text: "No cuentas con ningun producto en el inventario."
    }

    ItemSelectionModel {
        id: selectionModel
        model: root.productModel
        onSelectedIndexesChanged: {
            internal.isAcceptBtnEnabled = selectedIndexes.length > 0;
        }
    }

    Connections {
        target: selectionModel

        function onSelectionChanged(selectedList, deselectedList) {
            const forEachSelection = (selection, func) => {
                const topLeft = selection.topLeft;
                const row = topLeft.row;
                const product = topLeft.model.get(row);
                func(product)
            }

            if (selectedList.length > 0) {
                for (let selected of selectedList)
                    forEachSelection(selected, p => selectedProducts.append(p));
            }

            if (deselectedList.length > 0) {
                for (let deselected of deselectedList)
                    forEachSelection(deselected, p => (
                        selectedProducts.removeByProductId(p.productId)
                    ));
            }
        }
    }

    ProductProxyModel {
        id: proxyModel
        sourceModel: root.productModel
        productName: root.searchText
    }

    Connections {
        target: ProductInventoryController.catalog

        function onUpdated() {
            console.log("ProductInventoryController.catalog: Model updated")
            if (root.productModel.count === 0)
                root.state = "empty";
            else
                root.state = "";
        }
    }

    Component {
        id: productDelegate
        ProductSelectorDelegate {
            id: productThumbnail
            required index
            required model
            required productName
            required productId
            required quantityAvailable
            required reorderPoint
            required imageUrl

            enabled: quantityAvailable > 0
            background: Rectangle {
                color: {
                    if (!selectionModel.hasSelection)
                        return "transparent";
                    const selectedById = productModel.find(productId);
                    if (selectionModel.isSelected(selectedById))
                        return "cornflowerblue";
                    return "transparent";
                }
            }
            height: 100
            width: listView.width
            onClicked: {
                const selectedById = root.productModel.find(productId);
                selectionModel.select(selectedById, ItemSelectionModel.Select | ItemSelectionModel.Toggle);
            }
        }
    }

    SellerProductRepositoryREST {
        id: sellerProductRepository
    }

    QtObject {
        id: internal
        property bool isAcceptBtnEnabled
    }

    function removeSelectionOf(productId: int) {
        const idx = root.productModel.index(indexOf(productId), 0);
        if (!idx.valid) {
            print("The idx is not valid");
            return;
        }
        selectionModel.select(idx, ItemSelectionModel.Deselect);
    }

    function deselectAll() {
        selectionModel.clear();
    }

    function indexOf(productId: int): int {
        for (let i = 0; i < root.productModel.count; ++i)
            if (productModel.get(i).productId === productId)
                return i;
        return -1;
    }

    function updateModel(salespersonId: int) {
        sellerProductRepository.sellerId = salespersonId;
        const inventory = ProductInventoryController.inventory;
        inventory.repository = sellerProductRepository;
        inventory.update();
    }
}

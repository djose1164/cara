import QtQuick
import QtQuick.Controls
import QtQml.Models
import Cara.Inventory
import Cara.Product
import Cara.Controls

Item {
    id: root

    required property ProductModel selectedProducts
    required property string searchText
    property alias productModel: productModel

    states: [
        State {
            name: "fetching"
            PropertyChanges {
                target: busyIndicator
                running: true
            }
        },
        State {
            name: "empty"
            PropertyChanges {
                target: errorPane
                visible: true
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
        model: productModel
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
        sourceModel: productModel
        productName: searchText
    }

    ProductModel {
        id: productModel
        repository: inventoryRepository
        onUpdated: {
            if (count === 0)
                root.state = "empty";
            else
                root.state = "";
        }
    }

    InventoryRepository {
        id: inventoryRepository
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
                const selectedById = productModel.find(productId);
                selectionModel.select(selectedById, ItemSelectionModel.Select | ItemSelectionModel.Toggle);
            }
        }
    }

    QtObject {
        id: internal
        property bool isAcceptBtnEnabled
    }

    function removeSelectionOf(productId: int) {
        const idx = productModel.index(indexOf(productId), 0);
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
        for (let i = 0; i < productModel.count; ++i)
            if (productModel.get(i).productId === productId)
                return i;
        return -1;
    }

    function updateModel(salespersonId: int) {
        productModel.update(salespersonId);
    }
}

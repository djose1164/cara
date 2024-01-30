import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQml.Models
import Utils
import Inventory
import Billing
import Product
import "qrc:/utils"

Drawer {
    id: root

    required property int salespersonId
    required property ProductModel selectedProducts

    signal productSelected(products: ProductModel)

    interactive: false
    onAboutToHide: closeHandle()
    Component.onCompleted: {
        productModel.update(salespersonId);
    }

    SearchHeaderItem {
        id: header
        anchors.top: parent.top
        width: root.width
        height: 50
        title: "Seleccionar producto"
        onCloseClicked: root.close()
    }

    ListView {
        id: listView
        width: root.width
        anchors {
            top: header.bottom
            bottom: parent.bottom
        }
        model: proxyModel
        delegate: productDelegate
        clip: true
        spacing: 10
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
    }

    ItemSelectionModel {
        id: selectionModel
        model: productModel
        onSelectedIndexesChanged: {
            internal.isAcceptBtnEnabled = selectedIndexes.length > 0;
        }
    }

    ProductProxyModel {
        id: proxyModel
        sourceModel: productModel
        productName: header.displayText
    }

    ProductModel {
        id: productModel
        repository: inventoryRepository
        onUpdated: busyIndicator.running = false
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

            readonly property var modelIndex: productModel.index(index, 0)

            enabled: quantityAvailable > 0
            background: Rectangle {
                id: backRec
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

    function indexOf(productId: int): int {
        for (let i = 0; i < productModel.count; ++i)
            if (productModel.get(i).productId === productId)
                return i;
        return -1;
    }

    function closeHandle() {
        for (let selected of selectionModel.selectedIndexes) {
            const selectedAt = selected.model.get(selected.row);
            if (selectedProducts.contains(selectedAt))
                continue;
            selectedProducts.append(selectedAt);
        }

        root.close();
    }
}

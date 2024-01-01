import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQml.Models
import Utils
import Inventory
import "qrc:/product"
import "qrc:/utils"

Drawer {
    id: root

    required property int salespersonId
    required property ProductModel selectedProducts

    signal productSelected(products: ProductModel)

    interactive: false
    Component.onCompleted: {
        productModel.update(salespersonId);
    }

    DrawerHeader {
        id: header
        anchors.top: parent.top
        width: root.width
        title: "Seleccionar producto"
        menu.sourceComponent: RoundButton {
            id: acceptBtn
            enabled: internal.isAcceptBtnEnabled
            text: "Aceptar"
            onClicked: {
                for (let selected of selectionModel.selectedIndexes) {
                    const selectedAt = selected.model.get(selected.row);
                    if (selectedProducts.contains(selectedAt))
                        continue;
                    selectedProducts.append(selectedAt);
                }

                root.close();
            }
        }
    }

    ListView {
        id: listView
        width: root.width
        anchors {
            top: header.bottom
            bottom: parent.bottom
        }
        model: productModel
        delegate: productDelegate
        clip: true
        spacing: 10
    }

    ItemSelectionModel {
        id: selectionModel
        model: productModel
        onSelectedIndexesChanged: {
            internal.isAcceptBtnEnabled = selectedIndexes.length > 0;
        }
    }

    ProductModel {
        id: productModel
        repository: inventoryRepository
    }

    InventoryRepository {
        id: inventoryRepository
    }

    Component {
        id: productDelegate
        ProductSelectorDelegate {
            id: productThumbnail

            readonly property var modelIndex: productModel.index(index, 0)
            background: Rectangle{
                id: backRec
            }
            height: 100
            width: listView.width
            onClicked: {
                selectionModel.select(modelIndex, ItemSelectionModel.Select | ItemSelectionModel.Toggle);
            }

            Connections {
                target: selectionModel

                function onSelectionChanged() {
                    const idxIsSelected = selectionModel.isSelected(modelIndex);
                    if (idxIsSelected)
                        backRec.color = "cornflowerblue";
                    else
                        backRec.color = "transparent";
                }
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
}

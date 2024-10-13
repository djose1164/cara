pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQml.Models
import Cara.Product
import Cara.Controls
import Cara.ProductInventory
import Cara.Utils

Item {
    id: root

    required property ProductModel selectedProducts
    required property ProductModel model
    property alias searchText: proxyModel.productName
    property alias delegate: listView.delegate
    readonly property alias selectionModel: selectionModel

    signal closeClicked

    implicitWidth: 400
    implicitHeight: 500

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
                when: root.model.count === 0
                errorPane.visible: true
            }
        }
    ]

    SelectorHeaderItem {
        id: searchHeader
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        title: "Seleccionar producto"
        onCloseClicked: root.closeClicked()
    }

    ListView {
        id: listView
        width: root.width
        anchors.top: searchHeader.bottom
        anchors.bottom: parent.bottom
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
        model: root.model
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
        sourceModel: root.model
        productName: searchHeader.searchText
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
            text: `<h3>${productName}</h3>`
                  + `<p>Cantidad disponible: <strong>${quantityAvailable}</strong>`
            selection: selectionModel
            productModel: root.model
            height: 100
            width: root.width
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
        const idx = root.model.index(indexOf(productId), 0);
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
        for (let i = 0; i < root.model.count; ++i)
            if (root.model.get(i).productId === productId)
                return i;
        return -1;
    }
}

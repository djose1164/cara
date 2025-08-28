pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQml.Models
import Cara.Controls
import Cara.ProductInventory
import Cara.Utils

Item {
    id: root

    property ProductModel model
    property ItemSelectionModel selectionModel
    property alias searchText: proxyModel.productName
    property alias delegate: listView.delegate

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
            selection: root.selectionModel
            productModel: root.model
            height: 100
            width: root.width
        }
    }
}

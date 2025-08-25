import QtQuick
import QtQuick.Controls
import Cara.ProductInventory

ItemDelegate {
    id: root

    required property ItemSelectionModel selection
    required property ProductModel productModel
    property int index
    property var model
    property string productName
    property int productId
    property int quantityAvailable
    property int reorderPoint
    property string imageUrl

    contentItem: Row {
        spacing: 10
        Image {
            id: image
            source: root.imageUrl
            sourceSize.width: root.width /4
            sourceSize.height: root.height
            onStatusChanged: if (status === Image.Error) console.log(image.source)
        }

        Label {
            width: root.width - image.implicitWidth
            text: root.text
        }
    }

    background: Rectangle {
        color: {
            if (!root.selection.hasSelection)
                return "transparent";
            const selectedById = root.productModel.find(root.productId);
            if (root.selection.isSelected(selectedById))
                return "cornflowerblue";
            return "transparent";
        }
    }

    onClicked: {
        const selectedById = root.productModel.find(productId);
        selection.select(selectedById, ItemSelectionModel.Select | ItemSelectionModel.Toggle);
    }
}

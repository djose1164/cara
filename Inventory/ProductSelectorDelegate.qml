import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQml.Models
import Utils
import Inventory
import Product

ItemDelegate {
    id: productThumbnail
    property int index
    property var model
    property string productName
    property int productId
    property int quantityAvailable
    property int reorderPoint
    property string imageUrl

    Row {
        spacing: 10
        Image {
            id: image
            source: imageUrl
            sourceSize.width: productThumbnail.width /4
            sourceSize.height: productThumbnail.height
        }

        Label {
            width: productThumbnail.width - image.width
            text: `
            <h3>${productName}</h3>
            <p>Cantidad disponible: <strong>${quantityAvailable}</strong>
            `
        }
    }
}

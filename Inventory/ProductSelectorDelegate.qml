import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQml.Models
import Utils
import Inventory
import Product
import "qrc:/utils"

ItemDelegate {
    id: productThumbnail
    required property int index
    required property var model
    required property string productName
    required property int productId
    required property int quantityAvailable
    required property int reorderPoint
    required property string imageUrl

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

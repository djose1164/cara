import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQml.Models
import Cara.Utils
import Cara.Inventory
import Cara.Product

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
            onStatusChanged: if (status === Image.Error) console.log(image.source)
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

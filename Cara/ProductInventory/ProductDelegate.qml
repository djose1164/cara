import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import Cara.Shopping
import Cara.Network

Item {
    id: root
    required property var modelData
    readonly property int productId: modelData.id
    property string name: modelData.name
    property var images: modelData.images
    property string description: modelData.description
    property int sell_price: modelData.sell_price
    readonly property T.StackView stackView: ListView.view.stackView

    width: ListView.view.width
    height: 200

    StackLayout {
        id: stackLayout
        anchors.fill: parent
        anchors {
            leftMargin: 20
            rightMargin: 20
        }

        Item {
            BusyIndicator {
                anchors.centerIn: parent
            }
        }

        ItemDelegate {
            onClicked: stackView.push("qrc:/qt/qml/Cara/ProductInventory/ProductPage.qml",
                                      {productId, product: internal.product})
            Column {
            ProductImage {
                id: img

                source: root.images && root.images.length ? Api.uploadsPath + images[0].image_url : "qrc:/cara-logo-v1.png"
                sourceSize: "128x128"
                onReady: stackLayout.currentIndex = 1
            }

            Label {
                text: name
                font.pointSize: 14
                font.bold: true
            }

            Label {
                text: `RD$${sell_price}`
                font.pointSize: 12
            }
            }
        }
    }

    CatalogueRepository {
        id: catalogue
        Component.onCompleted: {
            readProductById(root.productId);
        }
        onProductFetched: resp => {
            const product = JSON.parse(resp).product;
            internal.product = product;
        }
    }

    QtObject {
        id: internal
        property var product
    }
}

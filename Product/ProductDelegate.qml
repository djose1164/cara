import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T

Item {
    id: root
    required property int id
    readonly property int productId: id
    required property string name
    required property string image_url
    required property string description
    required property int sell_price
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
            onClicked: stackView.push("qrc:/qt/qml/Product/ProductPage.qml",
                                      {sell_price, name, image_url, description,
                                        productId})
            Column {
            ProductImage {
                id: img

                source: image_url
                sourceSize: "128x128"//`${root.height-60}x${root.height-60}`
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
}

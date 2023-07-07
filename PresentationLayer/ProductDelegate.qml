import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    required property int id
    readonly property int productId: id
    required property string name
    required property string image_url
    required property string description
    required property int sell_price
    readonly property StackView stackView: ListView.view.stackView

    width: ListView.view.width * .55
    height: ListView.view.height

    StackLayout {
        id: stackLayout
        anchors.fill: parent

        Item {
            BusyIndicator {
                anchors.centerIn: parent
            }
        }

        ItemDelegate {
            onClicked: stackView.push("qrc:/product/ProductPage.qml",
                                      {sell_price, name, image_url, description,
                                        productId})
            ProductImage {
                id: img
                anchors.centerIn: parent
                source: image_url
                sourceSize: `${root.height-60}x${root.height-60}`
                onReady: stackLayout.currentIndex = 1
            }

            Label {
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                text: name
                font.pointSize: 16
            }
        }
    }
}

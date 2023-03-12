import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
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
            onClicked: stackView.push("qrc:/product/Product.qml",
                                      {sell_price, name, image_url, description})
            ColumnLayout {
                anchors.centerIn: parent
                spacing: 10
                Image {
                    id: img
                    source: image_url
                    sourceSize: `${root.height-50}x${root.height-50}`
                    onProgressChanged: if (img.progress === 1.0) stackLayout.currentIndex = 1
                }

                Label {
                    text: name
                    font.pointSize: 16
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                }
            }
        }
    }
}

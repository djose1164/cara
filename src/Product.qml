import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "qrc:/utils"

Page {
    id: root
    readonly property StackView stackView: StackView.view
    readonly property TabBar bar: stackView ? stackView.bar : null
    required property string name
    required property int sell_price
    required property string description
    required property string image_url
    required property int inStock

    header: TitleHeader {
        title: "Producto"
        stackView: root.stackView
    }

    StackLayout {
        id: stackLayout
        anchors.fill: parent

        Item {
            BusyIndicator {
                anchors.centerIn: parent
            }
        }

        Item {
            ColumnLayout {
                anchors.centerIn: parent
                spacing: 10

                Image {
                    id: img
                    source: image_url
                    sourceSize: `${root.width*.9}x${root.height*.4}`
                    onStatusChanged: if (img.status === Image.Ready) stackLayout.currentIndex = 1
                }

                StarRating {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: root.height*.1
                }

                Label {
                    text: name
                    font.pointSize: 14
                }

                Label {
                    text: inStock === 1 ? `¡Solo queda ${inStock}!`
                                        : inStock ? `¡Solo quedan ${inStock}!`
                                                  : `¡Agotado!`
                    visible: inStock <= 6
                    font.pointSize: 14
                }

                Label {
                    text: `Precio: RD$${sell_price}`
                    font.pointSize: 12
                }

                Label {
                    text: `Descripción: ${description}`
                    font.pointSize: 12
                }

                Button {
                    Layout.fillWidth: true
                    text: "Comprar"
                    onClicked: bar.currentIndex = 1
                    font.pointSize: 12
                }
            }
        }
    }
}

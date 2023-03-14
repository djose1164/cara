import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "qrc:/utils"

Page {
    id: root
    readonly property StackView stackView: StackView.view
    readonly property TabBar bar: stackView.bar
    required property string name
    required property int sell_price
    required property string description
    required property string image_url

    header: TitleHeader {
        title: "Producto"
        stackView: root.stackView
    }

        ColumnLayout {
            anchors.centerIn: parent

            Image {
                id: img
                source: image_url
                sourceSize: `${root.width*.9}x${root.height*.4}`
            }

            Label {
                text: name
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

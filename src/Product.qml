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
        anchors.fill: parent

        Image {
            id: img
            source: image_url
            sourceSize: `${root.width/2}x${root.height/2}`
        }

        Label { text: name }

        Label { text: `Precio: RD$${sell_price}` }

        Label { text: `Descripción: ${description}` }

        Button {
            text: "Comprar"
            onClicked: bar.currentIndex = 1
        }
    }
}

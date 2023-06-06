import QtQuick
import QtQuick.Controls
import "qrc:/utils"
import "qrc:/product"

Page {
    id: root

    readonly property StackView stackView: StackView.view

    title: "Inventario"
    header: TitleHeader {
        title: root.title
        stackView: root.stackView
    }

    ProductView {
        id: productView
        anchors.fill: parent
        anchors.topMargin: 5
        delegate: Item{
            id: item
            required property string name
            required property string image_url
            required property int id
            required property int sell_price
            required property var stock
            width: root.width
            height: childrenRect.height
            ProductThumbnail {
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.right: parent.right
                anchors.rightMargin: 5

                width: parent.width
                height: 75
                detailed: `Disponible ${stock.in_stock}`
                source: image_url
                name: item.name
        }
        }
    }
}

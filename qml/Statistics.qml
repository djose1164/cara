import QtQuick
import QtQuick.Controls
import "qrc:/utils"
import "qrc:/product"

Page {
    id: root

    readonly property var stackView: StackView.view

    title: "Estadisticas"
    header: TitleHeader {
        title: root.title
        onBackClicked: stackView.pop()
    }

    GridView {
        id: gridView
        anchors {
            fill: parent
            margins: 10
        }
        cellWidth: 190
        cellHeight: 190
        model: model
        delegate: ItemDelegate {
            width: 180
            height: 180

            onClicked: stackView.push(path, {headerTitle: title})

            Column {
                anchors.fill: parent
                padding: 24

                Image {
                    id: image
                    source: imageSource
                    sourceSize: "100x100"
                }

                Label {
                    width: image.width
                    topPadding: 12
                    text: title
                    font.pointSize: 12
                    wrapMode: Label.WordWrap
                }
            }
        }
    }

    ListModel {
        id: model

        ListElement {
            imageSource: "qrc:/icons/product_statistics.svg"
            title: "Estadisticas de Productos"
            path: "qrc:/statistics/ProductStatistics.qml"
        }

        ListElement {
            imageSource: "qrc:/icons/customer_statistics.svg"
            title: "Estadisticas de Clientes"
            path: "qrc:/statistics/CustomerStatistics.qml"
        }
    }
}

import QtQuick
import QtQuick.Controls
import "qrc:/utils"
import "qrc:/product"

Item {
    id: root

    readonly property var stackView: StackView.view
    property string headerTitle

    Component.onCompleted: statisticsApi.fetchStatistics()

    Header {
        id: header
        title: headerTitle
    }

    ListView {
        id: listView

        width: root.width
        height: root.height - header.height
        anchors.top: header.bottom
        header: Label {
            width: root.width
            text: "Mas vendidos"
            padding: 10
            font.pointSize: 14
            font.bold: true
            wrapMode: Label.Wrap
        }
        model: listModel
        delegate: ProductThumbnail {
            required name
            required property string image_url
            required property int quantity

            width: root.width
            source: image_url
            detailed: `Cantidad vendida: ${quantity}`
        }

        BusyIndicator {
            anchors.centerIn: parent
            visible: root.count === 0
        }
    }

    ListModel { id: listModel }

    StatisticsAPI {
        id: statisticsApi

        onMostSellingProductsFetched: products => listModel.append(products)
    }
}

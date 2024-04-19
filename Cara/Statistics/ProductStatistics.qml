import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import Cara.Utils
import Cara.Product

Item {
    id: root

    required property int salespersonId
    readonly property T.StackView stackView: StackView.view
    property string headerTitle

    Component.onCompleted: statisticsApi.fetchMostSellingProducts()

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
        spacing: 20
        model: listModel
        delegate: ProductThumbnail {
            required name
            required property string image_url
            required property string sold_quantity

            height: 100
            width: root.width
            source: image_url
            detailed: `Cantidad vendida: ${sold_quantity}`
        }

        BusyIndicator {
            anchors.centerIn: parent
            visible: root.count === 0
        }
    }

    ListModel {
        id: listModel
    }

    StatisticsAPI {
        id: statisticsApi
        salespersonId: root.salespersonId
        onMostSellingProductsFetched: products => products.forEach(e => listModel.append(e))
    }
}

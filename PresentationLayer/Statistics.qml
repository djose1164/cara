import QtQuick
import QtQuick.Controls
import "qrc:/utils"
import "qrc:/product"

HeaderPage {
    id: root

    Component.onCompleted: statisticsApi.fetchMostSellingProducts()

    title: "Estadisticas"

    ListView {
        id: listView
        anchors.fill: parent
        header: Label {
            text: "Productos mas vendidos"
            padding: 10
            font.pointSize: 14
            font.bold: true
        }

        model: model
        delegate: ProductThumbnail {
            required name
            required property string image_url
            required property int quantity

            width: listView.width
            source: image_url
            detailed: `Cantidad vendida: ${quantity}`
        }
    }


    ListModel { id: model }

    CaraApi {
        id: statisticsApi
        endpoint: "/statistics"

        signal mostSellingProductsFetched(products: var)

        onMostSellingProductsFetched: products => model.append(products)

        function fetchMostSellingProducts() {
            get("/");
            succeeded.connect(function(resp) {
                mostSellingProductsFetched(resp.statistics);
            });
        }
    }
}

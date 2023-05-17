import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/product"

Item {
    id: root

    property StackView stackView

    width: 400
    height: 100

    Label {
        id: label
        anchors {
            top: parent.top
            topMargin: 10
            left: parent.left
            leftMargin: 5
        }
        text: "¡Aprovecha las ofertas!"
        font.pointSize: 13
    }

    StackLayout {
        id: offerStackLayout
        width: parent.width
        height: root.height - label.height
        anchors.top: label.bottom

        BusyIndicator {}

        ListView {
            id: listView
            readonly property StackView stackView: root.stackView

            model: productRequest.model
            delegate: ProductDelegate {}
            orientation: ListView.Horizontal
        }
    }

    ProductRequest {
        id: productRequest
        onSucceeded: offerStackLayout.currentIndex = 1
    }

    function addSuggetionsToModel() {
        productRequest.addSuggetionsToModel();
    }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/singleton"

Item {
    id: root

    property alias delegate: listView.delegate
    property alias spacing: listView.spacing
    property alias listView: listView

    Component.onCompleted: if (ProductManager.modelIsEmpty()) productRequest.addSuggetionsToModel()

    ListView {
        id: listView

        readonly property StackView stackView: root.stackView

        anchors.fill: parent
        spacing: 20
        model: ProductManager.model

        BusyIndicator {
            anchors.centerIn: parent
            visible: listView.count === 0
        }
    }

    ProductRequest {
        id: productRequest
        onSucceeded: products => ProductManager.modelAppend(products);
    }

    Connections {
        target: ProductManager

        function onUpdateModel() {
            ProductManager.modelClear();
            productRequest.addSuggetionsToModel();
        }
    }
}


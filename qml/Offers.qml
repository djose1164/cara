import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import Cara.Product

Item {
    id: root

    property T.StackView stackView

    Component.onCompleted: productRequest.addSuggetionsToModel()

    StackLayout {
        id: offerStackLayout
        width: root.width
        height: root.height

        BusyIndicator {}

        ListView {
            id: listView
            readonly property T.StackView stackView: root.stackView

            spacing: 10
            model: model
            clip: true
            delegate: ProductDelegate {}
            section.property: "category_name"
            section.delegate: sectionDelegate
        }
    }

    Component {
        id: sectionDelegate
        Rectangle {
            required property var section
            width: root.width
            height: 40

            Label {
                anchors.verticalCenter: parent.verticalCenter
                text: section
                font.pointSize: 14
                font.bold: true
                padding: 20
            }
        }
    }

    ProductRequest {
        id: productRequest
        onSucceeded: function (products) {
            products.forEach(e => model.append(e));
            offerStackLayout.currentIndex = 1;
        }
    }

    ListModel { id: model }
}

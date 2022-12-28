import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/product" as Products

Item {
    property alias placeholder: productField.placeholderText

    //property alias toFind: productField.busyIndicator

    GridLayout {
        id: gridLayout
        columns: 2
        columnSpacing: 0
        rowSpacing: 0
        width: parent.width
        height: 60

        TextField {
            id: productField
            placeholderText: "Nombre del producto"
            Layout.fillWidth: true
            Keys.onReturnPressed: productRequest.addSuggetionsToModel()
            Layout.alignment: Qt.AlignTop
            Layout.margins: 0
            Layout.leftMargin: 10
        }

        ToolButton {
            background: Image {
                antialiasing: false
                source: "qrc:/icons/search.svg"
                sourceSize: iconSize
            }
            onClicked: productRequest.addSuggetionsToModel()
            Layout.alignment: Qt.AlignTop
            Layout.margins: 0
            Layout.rightMargin: 10
        }

        BusyIndicator {
            id: busyIndicator
            Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
            Layout.columnSpan: 2
            Layout.margins: 0
            running: false
        }

    }

    ListView {
        height: parent.height - gridLayout.height
        width: parent.width
        anchors.top: gridLayout.bottom
        anchors.topMargin: 5
        model: productModel
        delegate: ProductDelegate {}
    }

    ListModel { id: productModel }

    Products.ProductRequest {
        id: productRequest
        model: productModel
        //busyIndicator: busyIndicator_
        text: productField.text
    }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    property alias placeholder: productField.placeholderText

    GridLayout {
        columns: 2
        columnSpacing: 0
        rowSpacing: 0
        width: parent.width
        height: 60

        TextField {
            id: productField
            placeholderText: "Nombre del producto"
            Layout.fillWidth: true
            Keys.onReturnPressed: addSuggetions(text, busyIndicator)
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
            onClicked: addSuggetions(productField.text, busyIndicator)
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

    ListModel { id: productModel }

    WorkerScript {
        id: searchWorker
        source: "qrc:/js/product_request.mjs"
        property var busyIndicator
        onMessage: busyIndicator.running = false
    }

    function addSuggetions(text, busyIndicator) {
        searchWorker.busyIndicator = busyIndicator
        busyIndicator.running = true;
        searchWorker.sendMessage({"name": text, "model": productModel});
    }
}

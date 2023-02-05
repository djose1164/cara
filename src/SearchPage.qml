import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/product" as Products

Page {

    header: SearchBar {}

    GridLayout {
        id: gridLayout
        columns: 2
        columnSpacing: 0
        rowSpacing: 0
        width: parent.width
        height: 60

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
        model: productRequest.model
        delegate: ProductDelegate {}
    }



    Products.ProductRequest {
        id: productRequest
        //busyIndicator: busyIndicator_
        //text: productField.text
    }
}

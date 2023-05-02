import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/product" as Products

Page {
    id: root
    property string placeholderText: ""
    readonly property StackView stackView: StackView.view

    signal acceptedText(text: string)

    title: "Buscador"
    header: SearchBar {
        id: searchBar
        stackView: root.stackView
        placeholderText: root.placeholderText ? root.placeholderText : "Buscar en Cara..."
        onAccepted: t => productsModel.findProductByIdentifier(t)
    }

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
        id: listView
        readonly property StackView stackView: root.stackView
        height: parent.height - gridLayout.height
        width: parent.width
        anchors.top: gridLayout.bottom
        anchors.topMargin: 5
        model: productsModel.model
        delegate: Products.ProductDelegate {
            width: listView.width; height: listView.height/2
        }
    }

    Products.ProductRequest { id: productsModel }


}

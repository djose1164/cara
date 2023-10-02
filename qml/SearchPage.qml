import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/product" as Products

Page {
    id: root
    property string placeholderText: ""
    readonly property var stackView: StackView.view

    signal acceptedText(text: string)

    title: "Buscador"
    header: SearchBar {
        id: searchBar
        loader.sourceComponent: SearchField {
            placeholderText: "Buscar en .."
            onAccepted: t => productsModel.findProductByIdentifier(t)
        }
        onBackClicked: stackView.pop()
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
        model: listModel
        delegate: Products.ProductDelegate {
            width: listView.width; height: listView.height/2
        }
    }

    ListModel {
        id: listModel
    }

    Products.ProductRequest {
        id: productsModel
        onSucceeded: products => clearAndAppend(products)
    }

    function clearAndAppend(products) {
        listModel.clear();
        listModel.append(products);
    }
}

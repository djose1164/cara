import QtQuick
import QtQuick.Controls
import "qrc:/search"
import "qrc:/customers"

Page {
    id: root

    readonly property StackView stackView: StackView.view
    required property CustomerModel customerModel

    header: SearchBar {
        id: searchBar
        stackView: root.stackView
        placeholderText: "Buscar en facturas..."
        onAccepted: t => findMatch(t)
    }

    ListView {
        id: listView
        width: parent.width
        height: parent.height
        anchors {
            left: parent.left
            leftMargin: 20
        }
        model: model
        delegate: BillDelegate { stackView: root.stackView }
    }

    ListModel { id: model }

    function findMatch(text) {
        model.clear();
        const fetched = customerModel.likeName(text);
        if (fetched)
            model.append(fetched);
        print("findMatch(text)" +fetched);
    }
}

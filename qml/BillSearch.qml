import QtQuick
import QtQuick.Controls
import "qrc:/search"
import "qrc:/customers"

Page {
    id: root

    readonly property var stackView: StackView.view
    required property ListModel model

    header: SearchBar {
        id: searchBar
        stackView: root.stackView
        placeholderText: "Buscar en facturas..."
        onAccepted: t => findMatch(t)
    }

    ListView {
        id: listView
        readonly property StackView stackView: root.stackView
        width: parent.width
        height: parent.height
        anchors {
            left: parent.left
            leftMargin: 20
        }
        model: resultModel
        delegate: BillDelegate {
            required name
            required property int customer_id

            customerId: customer_id

            Component.onCompleted: print(name+" "+customerId)
        }
    }

    ListModel { id: resultModel }

    function findMatch(text) {
        resultModel.clear();
        const fetched = likeName(text);
        if (fetched)
            resultModel.append(fetched);
        print("findMatch(text)" +fetched);
    }

    function likeName(text) {
        let matches = [];
        const predicate = c => {
            const name = c.name;
            return name.toLowerCase().includes(text.toLowerCase()) ? c : null;
        };
        for (let i = 0; i < model.count; ++i) {
            const fetched = predicate(model.get(i));
            if (fetched) {
                matches.push(fetched);
                print(JSON.stringify(fetched, null, 2))
            }
        }
        return matches;
    }
}

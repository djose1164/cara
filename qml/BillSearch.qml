import QtQuick
import QtQuick.Controls
import User
import "qrc:/search"
import "qrc:/customers"

Page {
    id: root

    readonly property StackView stackView: StackView.view
    required property ListModel model

    header: SearchBar {
        id: searchBar
        loader.sourceComponent: SearchField {
            placeholderText: "Buscar en facturas..."
            onAccepted: t => findMatch(t)
        }
        onBackClicked: stackView.pop()
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
            userType: userStore.user.userType
        }
    }

    LocalUserStore { id: userStore }

    ListModel { id: resultModel }

    function findMatch(text) {
        resultModel.clear();
        const fetched = likeName(text);
        if (fetched)
            resultModel.append(fetched);
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

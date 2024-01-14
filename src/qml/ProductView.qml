import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/singleton"
import "qrc:/inventory"

Item {
    id: root

    required property int salespersonId
    property alias delegate: listView.delegate
    property alias spacing: listView.spacing
    property alias listView: listView

    Component.onCompleted: inventoryApi.fetchAll()

    signal entriesAdded(total: int)

    ListView {
        id: listView
        readonly property bool isEmpty: count === 0

        anchors.fill: parent
        spacing: 20
        model: model

        BusyIndicator {
            id: busyIndicator
            anchors.centerIn: parent
            visible: listView.isEmpty
        }
    }

    ListModel { id: model }

    InventoryAPI {
        id: inventoryApi
        salespersonId: root.salespersonId
        onFetchedAll: function(resp) {
            model.append(resp.inventory);
            entriesAdded(model.count);
        }
    }
}


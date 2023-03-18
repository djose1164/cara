import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "qrc:/orders"
import "qrc:/product"

ScrollView {
    id: root
    readonly property alias count: entries.count
    readonly property int total: {
        let partial_total = 0;
        for (let i = 0; i < entries.count; ++i) {
            const current = entryValueAt(i);
            partial_total += current.quantity * current.unit_price;
        }
        return partial_total;
    }

    ListView {
        readonly property ListModel productsModel: productModel.model
        property alias entries: entries
        anchors.fill: parent
        spacing: 5
        model: numEntries
        delegate: Entry {
            onUpdated: (idx, entryValue) => entries.set(idx, entryValue)
            onDeleted: idx => pop(idx)
        }
    }

    ListModel { id: entries }

    ListModel {
        id: numEntries
        ListElement { idx: 0 }
    }

    ProductRequest {
        id: productModel
        Component.onCompleted: addSuggetionsToModel()
    }

    function addEntry(): void {
        numEntries.append({idx: numEntries.count});
    }

    function pop(idx) {
        if (numEntries.count === 1)
            return;
        numEntries.remove(idx);
        entries.remove(idx);
    }

    function entryAt(idx: int): var {
        return entries.get(idx);
    }

    function entryValueAt(idx: int): var {
        return entryAt(idx).entryValue;
    }

    function entryValues(): var {
        const data = [];
        for (let i = 0; i < count; ++i)
            data.push(entryValueAt(i));
        return data;
    }

    function resetEntries() {
        for (let i = count; i !== 0; --i)
            pop(i);
    }
}

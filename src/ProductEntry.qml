import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "qrc:/orders"

ScrollView {
    id: root
    property int _index: 0
    readonly property alias count: entries.count
    property int total
    signal updateTotal
    signal entryAdded(idx: int, entry: var)
    signal entryDeleted(idx: int)

    onEntryAdded: (idx, entry) => {
        entries.set(idx, entry);
        total = calculateTotal();
    }
    onEntryDeleted: pop()

    anchors.fill: parent

    ListView {
        width: parent.width *.9
        height: parent.height *.9
        anchors.top: root.top
        spacing: 15
        model: entries
        delegate: Entry {
            Component.onCompleted: {
                added.connect(root.entryAdded);
                deleted.connect(root.entryDeleted);
            }
        }
    }

    ListModel {
        id: entries
        Component.onCompleted: append({"currentEntryIndex": _index++})
    }

    function addEntry(): void {
        entries.append({"currentEntryIndex": _index++});
        print(`index: ${_index}`)
    }

    function pop() {
        if (_index <= 1)
            return;
        entries.remove(--_index);
        total = calculateTotal();
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
        for (let i = _index; i != 0; --i)
            pop();
    }

    function calculateTotal(): int {
        let partial_total = 0;
        for (let i = 0; i < entries.count; ++i) {
            const current = entryValueAt(i);
            partial_total += current.quantity * current.unit_price;
        }
        return partial_total;
    }
}

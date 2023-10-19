import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "qrc:/orders"

ScrollView {
    id: root

    required property int valueRoleType
    readonly property alias count: entries.count
    readonly property alias model: entries

    signal entryUpdated
    Component.onCompleted: entries.append({quantity: 1, unit_price: 0, product_id: 0})

    ListView {
        property alias entries: entries
        anchors.fill: parent
        spacing: 5
        model: entries
        delegate: Entry {
            valueRoleType: root.valueRoleType
            onUpdated: (idx, entryValue) => {
                if (!entryValue.product_id && !entryValue?.unit_price)
                    return;
                entries.set(idx, entryValue);
                entryUpdated();
            }
            onDeleted: idx => {
                pop(idx);
                entryUpdated();
            }
        }
    }

    ListModel { id: entries }

    ProductRequest {
        id: productModel
        Component.onCompleted: addSuggetionsToModel()
    }

    function addEntry(): void {
        entries.append({idx: entries.count});
    }

    function pop(idx) {
        if (entries.count === 1)
            return;

        entries.remove(idx);
    }

    function entryAt(idx: int): var {
        return entries.get(idx);
    }

    function entryValues(): var {
        const data = [];
        for (let i = 0; i < count; ++i)
            data.push(newEntryValue(entryAt(i)));
        return data;
    }

    function newEntryValue(entry) {
        return JSON.parse(
            JSON.stringify(entry)
        )
    }

    function resetEntries() {
        entries.clear();
        addEntry();

    }

    function selectedProductsAreValid() {
        for (let i = 0; i < entries.count; ++i)
            if (!entryAt(i).product_id)
                return false;

        return true;
    }
}

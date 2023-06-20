import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "qrc:/orders"

ScrollView {
    id: root

    readonly property alias count: entries.count
    readonly property int total: {
        let partial_total = 0;
        for (let i = 0; i < entries.count; ++i) {
            const current = entries.get(i);
            partial_total += current.quantity * current.unit_price;
        }
        return partial_total;
    }

    Component.onCompleted: entries.append({quantity: 1, unit_price: 0, product_id: 0})

    ListView {
        readonly property ListModel productsModel: productModel.model
        property alias entries: entries
        anchors.fill: parent
        spacing: 5
        model: entries
        delegate: Entry {
            onUpdated: (idx, entryValue) => entries.set(idx, entryValue)
            onDeleted: idx => pop(idx)
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
        for (let i = count; i !== 0; --i)
            entries.remove(idx);
    }

    function selectedProductsAreValid() {
        for (let i = 0; i < entries.count; ++i)
            if (!entryAt(i).product_id)
                return false;

        return true;
    }
}

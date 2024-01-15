import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Controls.Material
import Customer
import "qrc:/orders"
import "qrc:/singleton"
import "qrc:/utils"

// TODO: Update implementation to use the new types.
Column {
    id: root

    property alias valueRoleType: productEntry.valueRoleType
    property int payment: 2
    property var details
    readonly property alias entriesCount: productEntry.count
    readonly property alias productEntries: productEntry
    readonly property int totalToPay: calculateTotal()

    signal okClicked
    signal updateTotal
    signal entryUpdated

    spacing: 20
    Component.onCompleted: payment = 0
    onOkClicked: productEntry.okClicked()

    ProductEntry {
        id: productEntry
        Component.onCompleted: entryUpdated.connect(root.entryUpdated)
        width: root.width
        height: root.height - newEntryBtn.height
    }

    RoundButton {
        id: newEntryBtn
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter

        icon.source: "qrc:/icons/add_circle.svg"
        icon.width: 36; icon.height: 36
        flat: true
        onClicked: productEntry.addEntry()
    }

    function findIn(model, criteria) {
        for (let i = 0; i < model.count; ++i)
            if (criteria(model.get(i)))
                return model.get(i);
        return null;
    }

    function clear() {
        payment = 0;
        productEntries.resetEntries();
    }

    function entryFormData(): var {
        return productEntries.entryValues();
    }

    function calculateTotal() {
        let partial_total = 0;
        for (let i = 0; i < productEntries.model.count; ++i) {
            const current = productEntries.model.get(i);
            partial_total += current.quantity * current.unit_price;
        }
        return partial_total;
    }
}

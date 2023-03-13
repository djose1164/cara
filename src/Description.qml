import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/customers"
import "qrc:/product"

Item {
    id: root
    property int payment: 2
    property int _index: 0
    property var details
    property int total: {
        let partial_total = 0;
        for (let i = 0; i < orderModel.count; ++i) {
            const current = orderModel.get(i).entryValue;
            partial_total += current.quantity * current.unit_price;
        }
        return partial_total;
    }

    signal okClicked
    signal updateTotal

    Component.onCompleted: payment = 0
    onOkClicked: productEntry.okClicked()

    ProductEntry {
        id: productEntry
        width: parent.width
        height: parent.height * .35
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 20
    }

    RowLayout {
        width: parent.width
        anchors.top: productEntry.bottom
        anchors.topMargin: 50
        spacing: 10

        Button {
            text: "Agregar"
            onClicked: repeaterModel.append({"currentEntryIndex": _index++})
            Layout.fillWidth: true
        }

        Button {
            text: "Remover"
            onClicked: if (_index > 1) removeEntryAt(--_index)
            Layout.fillWidth: true
        }
    }

    ListModel {
        id: repeaterModel
        Component.onCompleted: repeaterModel.append({"currentEntryIndex": _index++})
    }

    function findIn(model, criteria) {
        for (let i = 0; i < model.count; ++i)
            if (criteria(model.get(i)))
                return model.get(i);
        return null;
    }

    // Reset entires to one
    function resetEntries() {
        for (let i = _index; i != 0; --i)
            removeEntryAt(i);
    }

    function removeEntryAt(idx) {
        repeaterModel.remove(idx);
    }

    function clear() {
        payment = 0;
    }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Controls.Material
import "qrc:/customers"
import "qrc:/orders"
import "qrc:/singleton"
import "qrc:/utils"

// TODO: Update implementation to use the new types.
Column {
    id: root
    property int totalToPay: total
    property alias messageDialog: message

    property int payment: 2
    property var details
    property int total: productEntry.total
    readonly property alias entriesCount: productEntry.count
    readonly property alias productEntries: productEntry

    signal okClicked
    signal updateTotal

    Component.onCompleted: payment = 0
    onOkClicked: productEntry.okClicked()

    ProductEntry {
        id: productEntry
        width: root.width
        height: root.height - pane.height
    }

    Pane {
        id: pane
        width: root.width

        RoundButton {
            onClicked: productEntry.addEntry()

            z: 100
            icon.source: "qrc:/icons/add_circle.svg"
            icon.width: 36; icon.height: 36
            flat: true
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    function findIn(model, criteria) {
        for (let i = 0; i < model.count; ++i)
            if (criteria(model.get(i)))
                return model.get(i);
        return null;
    }

    function clear() {
        payment = 0;
    }

    MessageDialog {
        id: message
        buttons: MessageDialog.Ok
        onButtonClicked: close()
    }

    /**
    * Clear & prepare the form to be used on another order.
    */
    function clearOrderForm() {
        productEntries.resetEntries();
        clear();
    }

    function entryFormData(): var {
        return productEntries.entryValues();
    }
}

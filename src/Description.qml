import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/customers"
import "qrc:/product"

Item {
    id: root
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
        width: parent.width
        height: parent.height * .8
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
    }

    RowLayout {
        width: parent.width
        height: parent.height*.1
        anchors.top: productEntry.bottom
        anchors.topMargin: 10
        spacing: 10

        Button {
            text: "Agregar"
            onClicked: productEntry.addEntry()
            Layout.fillWidth: true
        }

        Button {
            text: "Remover"
            onClicked: productEntry.pop()
            Layout.fillWidth: true
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
}

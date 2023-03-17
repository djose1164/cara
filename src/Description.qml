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
    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        ProductEntry {
            id: productEntry
            Layout.fillWidth: true
            Layout.leftMargin: 5
            Layout.maximumHeight: root.height/2
            Layout.alignment: Qt.AlignTop
        }

        RoundButton {
            onClicked: productEntry.addEntry()
            icon.source: "qrc:/icons/add_circle.svg"
            icon.width: 36; icon.height: 36
            flat: true
            Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
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

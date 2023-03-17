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

    RoundButton {
        width: parent.height*.25
        height: parent.height*.25
        anchors.top: productEntry.bottom
        anchors.horizontalCenter: root.horizontalCenter

        onClicked: productEntry.addEntry()
        IconImage {
            anchors.centerIn: parent
            source: "qrc:/icons/add_circle.svg"
        }
        flat: true
        Layout.alignment: Qt.AlignCenter
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

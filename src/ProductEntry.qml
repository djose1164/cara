import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "qrc:/orders"

ScrollView {
    id: root
    signal updateTotal
    signal okClicked
    width: parent.width

    Component.onCompleted: {
        entry.entryAdded.connect(root.updateTotal);
        entry.okClicked.connect(root.okClicked);
    }
    ListView {
        width: parent.width *.9
        height: parent.height *.9
        anchors.centerIn: parent
        spacing: 15
        model: repeaterModel
        delegate: Entry { id: entry }
    }
}

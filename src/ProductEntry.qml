import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "qrc:/orders"

ScrollView {
    id: root
    signal updateTotal
    signal okClicked

    ColumnLayout {
        anchors.fill: parent
        anchors.topMargin: 15
        spacing: 15

        Repeater {
            model: repeaterModel

            Entry {
                Component.onCompleted: {
                    entryAdded.connect(root.updateTotal);
                    okClicked.connect(root.okClicked);
                }
            }
        }
    }
}

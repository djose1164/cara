import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "qrc:/orders"

ScrollView {
    id: root
    signal updateTotal
    width: parent.width

    ListView {
        width: parent.width *.9
        height: parent.height *.9
        anchors.centerIn: parent
        spacing: 15
        model: repeaterModel
        delegate: Entry {}
    }
}

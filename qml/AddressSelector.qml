import QtQuick
import QtQuick.Controls

Item {
    id: root

    property alias text: label.text
    readonly property alias comboBox: comboBox
    readonly property alias model: listModel

    width: 250
    height: 70

    Label {
        id: label
    }

    ComboBox {
        id: comboBox
        anchors {
            top: label.bottom
            topMargin: 5
        }
        width: root.width
        height: root.height*.8
        textRole: "name"
        valueRole: "id"
        model: listModel
        currentIndex: 0
    }

    ListModel { id: listModel }
}

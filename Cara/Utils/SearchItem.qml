import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import Cara.Controls

Item {
    id: root

    readonly property alias leadingBtn: leadingBtn
    readonly property alias displayText: searchField.displayText
    property alias background: background

    signal searchClosed
    signal textEdited

    implicitWidth: rowLayout.implicitWidth
    implicitHeight: 50

    RowLayout {
        id: rowLayout
        anchors.fill: parent
        z: 20

        ToolButton {
            id: leadingBtn
            icon.source: "qrc:/left_arrow"
            Layout.alignment: Qt.AlignVCenter | Qt.AlignLeading
            onClicked: root.endSearch()
        }

        SearchField {
            id: searchField
            placeholderText: "Buscar"
            Layout.rightMargin: rowLayout.spacing
            Layout.fillWidth: true
            Layout.topMargin: rowLayout.spacing
            Layout.bottomMargin: rowLayout.spacing
            onTextEdited: root.textEdited()
        }

        Item {}
    }

    Rectangle {
        id: background
        anchors.fill: parent
        color: "white"
        z: 10
    }

    function endSearch() {
        root.state = "";
        searchField.text = "";
        searchClosed();
    }
}

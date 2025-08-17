import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import Cara.Controls

Item {
    id: root

    readonly property alias leadingBtn: leadingBtn
    readonly property alias displayText: searchField.displayText

    signal searchClosed
    signal textEdited

    implicitWidth: rowLayout.implicitWidth
    implicitHeight: 50

    RowLayout {
        id: rowLayout
        anchors.fill: parent
        ToolButton {
            id: leadingBtn
            icon.source: "qrc:/left_arrow"
            Layout.alignment: Qt.AlignVCenter | Qt.AlignLeading
            onClicked: root.endSearch()
        }

        SearchField {
            id: searchField
            placeholderText: "Buscar"
            Layout.fillHeight: true
            Layout.rightMargin: rowLayout.spacing
            Layout.fillWidth: true
            Layout.topMargin: rowLayout.spacing
            Layout.bottomMargin: rowLayout.spacing
            onTextEdited: root.textEdited()
        }

        Item {}
    }

    function endSearch() {
        root.state = "";
        searchField.text = "";
        searchClosed();
    }
}

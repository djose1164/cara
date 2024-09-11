import QtQuick
import QtQuick.Controls
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
            Layout.maximumHeight: root.height * .7
            Layout.rightMargin: rowLayout.spacing
            Layout.fillWidth: true
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

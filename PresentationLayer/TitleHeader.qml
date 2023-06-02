import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ToolBar {
    property alias title: lbl.text
    property StackView stackView
    property alias label: lbl
    property alias moreVertVisible: moreVert.visible

    RowLayout {
        id: row
        anchors.fill: parent

        ToolButton {
            id: leftArrowBtn
            Layout.alignment: Qt.AlignLeft
            onClicked: stackView.pop()
            icon.source: "qrc:/left_arrow"
            visible: stackView
        }

        Label {
            id: lbl
            font.bold: true
            font.pointSize: 14
            horizontalAlignment:Label.AlignHCenter
            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
        }

        ToolButton {
            id: moreVert
            Layout.alignment: Qt.AlignRight
            icon.source: "qrc:/icons/more_vert.svg"
        }
    }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ToolBar {
    property alias title: lbl.text
    property StackView stackView
    property alias label: lbl

    RowLayout {
        anchors.fill: parent

        ToolButton {
            Layout.alignment: Qt.AlignLeading
            icon.source: "qrc:/left_arrow"
            onClicked: stackView.pop()
        }

        Label {
            id: lbl
            font.bold: true
            font.pointSize: 14
            horizontalAlignment: Label.AlignHCenter
           // Layout.alignment: Qt.AlignHCenter
        }
    }
}

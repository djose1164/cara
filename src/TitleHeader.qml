import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ToolBar {
    property alias title: lbl.text
    property StackView stackView
    property alias label: lbl

    RowLayout {
        readonly property int preferredMargin: 8

        anchors.fill: parent
        anchors.leftMargin: preferredMargin
        anchors.rightMargin: preferredMargin

        ToolButton {
            Layout.alignment: Qt.AlignLeading
            background: IconImage {
                source: "qrc:/left_arrow"
                sourceSize: iconSize
                antialiasing: false
            }
            onClicked: stackView.pop()
        }

        Label {
            id: lbl
            font.bold: true
            font.pointSize: 14
            Layout.alignment: Qt.AlignHCenter
        }
    }
}

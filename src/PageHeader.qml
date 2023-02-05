import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ToolBar {
    required property string title
    property StackView stackView

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
            text: title
            font.bold: true
            font.pointSize: 14
            Layout.alignment: Qt.AlignHCenter
        }

        ToolButton {
            background: IconImage {
                source: "qrc:/icons/settings.svg"

            }
            Layout.alignment: Qt.AlignTrailing
        }
    }
}

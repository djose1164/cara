import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ToolBar {
    id: root

    required property StackView stackView

    Rectangle {
        anchors.fill: parent
        color: "floralwhite"
    }

    RowLayout {
        anchors.fill: parent

        ToolButton {
            Layout.alignment: Qt.AlignVCenter
            background: IconImage {
                source: "qrc:/left_arrow"
                sourceSize: iconSize
                antialiasing: false
            }
            onClicked: stackView.pop()
        }
    }
}

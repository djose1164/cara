import QtQuick.Controls

ToolButton {
    id: root

    property StackView stackView

    onClicked: if (stackView) stackView.pop()

    icon.source: "qrc:/left_arrow"
}

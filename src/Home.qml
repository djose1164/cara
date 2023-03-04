import QtQuick
import QtQuick.Controls

Page {
    id: root
    readonly property StackView stackView: StackView.view

    header: HomeHeader { stackView: root.stackView }
}

import QtQuick
import QtQuick.Controls

Page {
    id: root
    readonly property var stackView: StackView.view

    header: HomeHeader { stackView: root.stackView }
}

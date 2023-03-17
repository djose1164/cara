import QtQuick
import QtQuick.Controls

Page {
    id: root
    readonly property StackView stackView: StackView.view
    readonly property TabBar bar: StackView.view.bar

    header: HomeHeader { stackView: root.stackView }
}

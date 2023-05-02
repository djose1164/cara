import QtQuick
import QtQuick.Controls

Page {
    id: root
    readonly property StackView stackView: StackView.view
    readonly property TabBar bar: stackView.bar

    header: HomeHeader { stackView: root.stackView }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
Page {
    id: root
    readonly property StackView stackView: StackView.view
    readonly property TabBar bar: stackView.bar
    property bool inHome: false
    signal homeClicked

    onHomeClicked: inHome ? stackView.pop(null) : inHome = true;

    header: HomeHeader { stackView: root.stackView }
}

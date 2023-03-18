import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
Page {
    id: root
    readonly property StackView stackView: StackView.view
    readonly property TabBar bar: stackView.StackLayout.bar
    property bool inHome: false
    signal homeClicked

    onHomeClicked: {
        if (inHome)
            for (let depth = stackView.depth; depth > 1; --depth)
                stackView.pop();
        else
            inHome = true;
    }

    header: HomeHeader { stackView: root.stackView }
}

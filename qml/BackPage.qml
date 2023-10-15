import QtQuick.Controls

Page {
    id: root

    readonly property StackView stackView: StackView.view

    header: TitleHeader {
        title: root.title
        onBackClicked: stackView.pop()
    }
}

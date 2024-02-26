import QtQuick.Controls

Page {
    id: root

    readonly property StackView stackView: StackView.view
    readonly property alias btnLoader: titleHeader.btnLoader

    header: TitleHeader {
        id: titleHeader
        title: root.title
        onBackClicked: stackView.pop()
    }
}

import QtQuick.Controls
import QtQuick.Templates as T

Page {
    id: root

    readonly property T.StackView stackView: StackView.view
    readonly property alias btnLoader: titleHeader.btnLoader

    header: TitleHeader {
        id: titleHeader
        title: root.title
        onBackClicked: stackView.pop()
    }
}

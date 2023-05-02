import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root
    readonly property StackView stackView: StackView.view
    header: TitleHeader {
        stackView: root.stackView
        title: root.title
    }
}

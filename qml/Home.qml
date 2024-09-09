import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import QtNetwork

Page {
    id: root
    readonly property T.StackView stackView: StackView.view

    header: HomeHeader { stackView: root.stackView }
}

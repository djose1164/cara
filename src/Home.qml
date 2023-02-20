import QtQuick
import QtQuick.Controls

Page {
    readonly property StackView stackView: StackView.view

    header: HomeHeader { id: mainHeader }
}

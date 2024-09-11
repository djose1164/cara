import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root

    default property alias childrenContent: columnLayout.data

    readonly property alias scrollView: scrollView
    readonly property alias columnLayout: columnLayout

    ScrollView {
        id: scrollView
        anchors.fill: parent
        contentHeight: columnLayout.childrenRect.height

        ColumnLayout {
            id: columnLayout
            width: root.width * .95
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}

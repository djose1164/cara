import QtQuick
import QtQuick.Controls

Item {
    id: root

    property alias count: pageIndicator.count
    property alias currentIndex: pageIndicator.currentIndex
    property string displayText

    implicitWidth: implicitHeight*root.count
    implicitHeight: 60

    PageIndicator {
        id: pageIndicator
        anchors.fill: parent
        delegate: Column {
            required property int index
            width: pageIndicator.width/pageIndicator.count
            spacing: pageIndicator.spacing
            bottomPadding: pageIndicator.spacing
            CircleIndicator {
                width: height
                height: pageIndicator.height - label.height - pageIndicator.spacing
                anchors.horizontalCenter: parent.horizontalCenter
                isCurrent: pageIndicator.currentIndex === index
                seen: index < pageIndicator.currentIndex
            }

            Label {
                id: label
                width: parent.width * .8
                //anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                visible: index === pageIndicator.currentIndex
                text: root.displayText
                horizontalAlignment: Label.AlignHCenter
                elide: Qt.ElideRight
                font.pointSize: 11
            }
        }

        contentItem: Row {
            spacing: pageIndicator.spacing

            Repeater {
                model: pageIndicator.count
                delegate: pageIndicator.delegate
            }
        }
    }
}

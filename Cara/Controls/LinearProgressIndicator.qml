import QtQuick
import QtQuick.Controls

Item {
    id: root

    property alias count: pageIndicator.count
    property alias currentIndex: pageIndicator.currentIndex
    property string displayText

    implicitWidth: pageIndicator.implicitWidth
    implicitHeight: pageIndicator.implicitHeight

    PageIndicator {
        id: pageIndicator
        anchors.fill: parent
        delegate: CircleIndicator {
            required property int index
            isCurrent: pageIndicator.currentIndex === index
            seen: index < pageIndicator.currentIndex
        }

        contentItem: Row {
            spacing: 0

            Repeater {
                model: pageIndicator.count
                delegate: pageIndicator.delegate
            }
        }
    }
}

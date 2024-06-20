import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property bool seen
    property bool isCurrent

    property int prefferedHeight: root.height
    readonly property int prefferedWidth: root.prefferedHeight*5

    implicitWidth: image.implicitWidth
    implicitHeight: image.implicitHeight

    Image {
        id: image
        anchors.fill: parent
        // sourceSize.height: root.prefferedHeight
        // sourceSize.width: root.prefferedHeight
        source: root.isCurrent ? "selected-circle.png" : "circle.png"
        opacity: root.isCurrent || root.seen ? .95 : .45

        Behavior on opacity { OpacityAnimator { duration: 100 } }
    }
}

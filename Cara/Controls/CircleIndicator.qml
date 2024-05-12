import QtQuick
import QtQuick.Controls

Item {
    id: root

    property bool seen
    property bool isCurrent: true
    property int prefferedHeight: 64
    readonly property int prefferedWidth: root.prefferedHeight*5
    property alias displayText: label.text

    implicitWidth: image.implicitWidth
    implicitHeight: image.implicitHeight + label.implicitHeight
    opacity: root.isCurrent || root.seen ? .95 : .45

    StateGroup {
        id: stateGroup
        states: [
            State {
                name: "afterSelectedCircle"
                when: !root.seen && !root.isCurrent
                PropertyChanges {
                    target: image
                    source: "after-selected-circle.png"
                    //sourceSize.width: root.prefferedHeight*2
                }
            },
            State {
                name: "beforeSelectedCircle"
                when: root.seen
                PropertyChanges {
                    target: image
                    source: "before-selected-circle.png"
                    //sourceSize.width: root.prefferedHeight*2
                }
            }
        ]
    }

    Image {
        id: image
        //width: root.isCurrent ? height : root.prefferedWidth
        sourceSize.width: root.isCurrent ? root.prefferedHeight : root.prefferedWidth
        sourceSize.height: root.prefferedHeight
        source: "selected-circle.png"

        Behavior on opacity { OpacityAnimator { duration: 100 } }
    }

    Label {
        id: label
        width: root.width
        anchors.bottom: parent.bottom
        visible: root.isCurrent
        horizontalAlignment: Label.AlignHCenter
        elide: Qt.ElideRight
        font.pointSize: 14
    }
}

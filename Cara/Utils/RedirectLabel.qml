import QtQuick
import QtQuick.Controls

ItemDelegate {
    id: root

    property alias title: customerLbl.text

    contentItem: Item {
        Label {
            id: customerLbl
            anchors.verticalCenter: parent.verticalCenter
            wrapMode: Label.Wrap
            elide: Label.ElideRight
            font.pointSize: 12
        }

        IconImage {
            id: icon
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            source: "qrc:/icons/arrow_forward.svg"
            sourceSize: "24x24"
        }
    }
}

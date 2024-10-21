import QtQuick
import QtQuick.Controls

ItemDelegate {
    id: root

    property alias title: customerLbl.text

    contentItem: Item {
        id: myItem
        implicitWidth: customerLbl.implicitWidth + icon.implicitWidth
        implicitHeight: Math.max(customerLbl.implicitHeight, icon.implicitHeight)
        Label {
            id: customerLbl
            width: myItem.width - icon.width
            elide: Label.ElideRight
            font: root.font
        }

        Image {
            id: icon
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            source: "qrc:/icons/arrow_forward.svg"
            sourceSize: Qt.size(24, 24)
            opacity: enabled ? 1 : .4
        }
    }

    font.pointSize: 16
}

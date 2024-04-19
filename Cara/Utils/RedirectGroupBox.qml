import QtQuick
import QtQuick.Controls
import Cara.Utils

Item {
    id: root

    default property alias data: groupBox.contentData
    property alias title: groupBox.title
    property alias redirectText: redirectLbl.title

    signal redirectClicked

    GroupBox {
        id: groupBox
        anchors.fill: parent
        label: Label {
            x: groupBox.leftPadding
            width: groupBox.availableWidth
            text: root.title
            rightPadding: redirectLbl.width
            elide: Label.ElideRight
            font.pointSize: 14

            RedirectLabel {
                id: redirectLbl
                anchors.right: parent.right
                onClicked: root.redirectClicked()
            }
        }
    }
}

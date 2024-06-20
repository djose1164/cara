import QtQuick
import QtQuick.Controls
import Cara.Utils

Item {
    id: root

    default property alias data: groupBox.contentData
    property alias title: groupBox.title

    signal redirectClicked

    GroupBox {
        id: groupBox
        anchors.fill: parent
        label: ItemDelegate {
            width: groupBox.width
            contentItem: Item {
                implicitWidth: titleLbl.implicitWidth + icon.implicitWidth
                implicitHeight: Math.max(titleLbl.implicitHeight, icon.implicitHeight)
                Label {
                    id: titleLbl
                    x: groupBox.leftPadding
                    width: groupBox.availableWidth
                    text: root.title
                    rightPadding: icon.width
                    elide: Label.ElideRight
                    font.pointSize: 14
                }

                Image {
                    id: icon
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    source: "qrc:/icons/arrow_forward.svg"
                    sourceSize: "24x24"
                }
            }
            onClicked: root.redirectClicked()
        }
    }
}

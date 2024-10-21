import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property alias customerNameTxt: customerNameLbl.text
    property alias dateTxt: dateLbl.text
    property alias canDelete: commentMenuButton.canDelete

    signal deleteClicked

    implicitWidth: rowLayout.implicitWidth
    implicitHeight: rowLayout.implicitHeight

    RowLayout {
        id: rowLayout
        anchors.fill: parent
        spacing: 0

        Label {
            id: customerNameLbl
            Layout.preferredWidth: implicitWidth*1.1
            font.pointSize: 12
            font.bold: true
        }

        Label {
            id: dateLbl
            wrapMode: Label.Wrap
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft
            font.pointSize: 10
        }

        CommentMenuButton {
            id: commentMenuButton
            Layout.alignment: Qt.AlignRight
            onDeleteClicked: root.deleteClicked()
        }
    }
}

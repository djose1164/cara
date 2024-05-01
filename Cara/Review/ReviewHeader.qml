import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    id: root

    property alias customerNameTxt: customerNameLbl.text
    property alias dateTxt: dateLbl.text
    property alias canDelete: commentMenuButton.canDelete

    signal deleteClicked

    spacing: 0

    Label {
        id: customerNameLbl
        Layout.preferredWidth: implicitWidth*1.1
        font.pointSize: 12
        font.bold: true
    }

    Label {
        id: dateLbl
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

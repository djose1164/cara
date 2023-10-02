import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import core

ItemDelegate {
    id: root

    property int customerId
    property alias name: nameLbl.text

    ColumnLayout {
        anchors.fill: parent

        Label {
            id: nameLbl
            Layout.topMargin: 15
            text: name
            font.pointSize: 16
        }

        Label {
            text: `Cliente ID #${customerId}`
            font.pointSize: 12
            Layout.leftMargin: 15
        }
    }
}

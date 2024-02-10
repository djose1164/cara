import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


ItemDelegate {
    id: root

    required property int customerId
    required property string name

    ColumnLayout {
        anchors.fill: parent
        anchors.leftMargin: 10

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
